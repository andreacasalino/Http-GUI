#include "httplib.h"
#include <ColoredStream/ColoredStream.hpp>
#include <HttpGui/BadResponse.h>
#include <HttpGui/Definitions.h>
#include <HttpGui/Server.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

namespace gui {
namespace {
class StringMerger {
public:
  template <typename T1, typename T2, typename... Args>
  static std::string merge(const T1 &first, const T2 &second, Args... args) {
    std::stringstream stream;
    merge_(stream, first, second, args...);
    return stream.str();
  }

private:
  template <typename T, typename... Args>
  static void merge_(std::stringstream &stream, const T &element,
                     Args... args) {
    merge_(stream, element);
    merge_(stream, args...);
  }

  template <typename T>
  static void merge_(std::stringstream &stream, const T &element) {
    stream << element;
  }
};

static const colored_stream::Color INFO_COLOR =
    colored_stream::ClassicColor::GREEN;
static const std::string INFO_DESCR = "INFO";

static const colored_stream::Color ERROR_COLOR =
    colored_stream::ClassicColor::RED;
static const std::string ERROR_DESCR = "ERROR";

class Logger {
public:
  Logger(const std::string &fileName, const std::size_t port,
         const std::string &preamble)
      : preamble(preamble), port(port) {
    if (!fileName.empty()) {
      log_stream = std::make_optional<std::ofstream>(fileName);
      if (!log_stream->is_open()) {
        log_stream.reset();
      }
    }
  };

  void log_info(const std::string &message) {
    log(message, INFO_DESCR, INFO_COLOR);
  };

  void log_error(const std::string &message) {
    log(message, ERROR_DESCR, ERROR_COLOR);
  };

private:
  void log(const std::string &message, const std::string &level,
           const colored_stream::Color &level_color) {
    log(message, level, level_color, std::cout);
    if (log_stream) {
      log(message, level, level_color, *log_stream);
    }
  };

  void log(const std::string &message, const std::string &level,
           const colored_stream::Color &level_color, std::ostream &stream) {
    stream << '['
           << colored_stream::ColoredStream{level_color, level, ':', port}
           << ']' << preamble << "= " << message << std::endl;
  };

  const std::string preamble;
  std::optional<std::ofstream> log_stream;
  const std::size_t port;
};

httplib::Server::Handler
make_action_for_server_registering(std::mutex &action_mtx, Logger &logger,
                                   const std::string &action_name,
                                   const Action &action) {
  return [&action_mtx, &logger, &action_name,
          &action](const httplib::Request &req, httplib::Response &res) {
    std::scoped_lock lock(action_mtx);
    logger.log_info(StringMerger::merge("request name: ", action_name));
    logger.log_info(StringMerger::merge("request body: ", req.body));
    res.set_header("Access-Control-Allow-Origin", "*");
    nlohmann::json gui_response;
    try {
      nlohmann::json json_request = nlohmann::json::parse(req.body);
      gui::Request gui_request = json_request.get<gui::Request>();
      action(gui_request, gui_response);
      auto gui_response_string = gui_response.dump();
      logger.log_info(
          StringMerger::merge("request response: ", gui_response_string));
      res.set_content(gui_response_string, "text/plain");
      return;
    } catch (const std::exception &e) {
      gui_response = make_bad_response(e.what());
    } catch (...) {
      gui_response = make_bad_response("UNKNOWN");
    }
    auto gui_response_string = gui_response.dump();
    logger.log_error(
        StringMerger::merge("request response: ", gui_response_string));
    res.set_content(gui_response_string, "text/plain");
  };
}
} // namespace

void Server::run(const std::size_t port, const std::string &logFileName) {
  {
    std::scoped_lock lock(consumed_ports_mtx);
    auto it_port = consumed_ports.find(port);
    if (it_port != consumed_ports.end()) {
      std::stringstream message;
      message << std::to_string(port) << " is an already in use port";
      throw std::runtime_error{message.str()};
    }
    consumed_ports.emplace(port);
  }

  {
    std::scoped_lock lock(actions_mtx);
    if (!actions_init_done) {
      post_actions = getPOSTActions();
      get_actions = getGETActions();
      actions_init_done = true;
    }
  }

  httplib::Server svr;

  Logger logger_POST_actions(logFileName, port, "[POST]");
  Logger logger_GET_actions(logFileName, port, "[GET]");

  // register actions to server
  for (const auto &[name, action] : post_actions) {
    svr.Post(StringMerger::merge('/', name).c_str(),
             make_action_for_server_registering(
                 actions_execution_mtx, logger_POST_actions, name, action));
  }
  for (const auto &[name, action] : get_actions) {
    svr.Get(StringMerger::merge('/', name).c_str(),
            make_action_for_server_registering(
                actions_execution_mtx, logger_POST_actions, name, action));
  }
  // add stop action
  svr.Post(StringMerger::merge('/', STOP_SERVER_KEYWORD).c_str(),
           [&svr](const httplib::Request &req, httplib::Response &res) {
             svr.stop();
           });

  svr.listen("localhost", static_cast<int>(port));
}
} // namespace gui
