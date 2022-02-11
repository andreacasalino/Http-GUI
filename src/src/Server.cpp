#include "httplib.h"
#include <HttpGui/BadResponse.h>
#include <HttpGui/Server.h>
#include <sstream>
#include <ColoredStream/ColoredStream.hpp>
#include <iostream>

#include <optional>
#include <fstream>

namespace gui {
namespace {
    static const colored_stream::Color INFO_COLOR = colored_stream::ClassicColor::GREEN;
    static const colored_stream::Color ERROR_COLOR = colored_stream::ClassicColor::RED;

    class Logger {
    public:
        Logger(const std::string& fileName, const std::size_t port) : port(port) {
            if (!fileName.empty()) {
                log_stream = std::make_optional<std::ofstream>(fileName);
                if (!log_stream->is_open()) {
                    log_stream.reset();
                }
            }
        };

        void log(const std::string& info, const colored_stream::Color& color, const std::string& content) {
            log(info, color, content, std::cout);
            if (log_stream) {
                log(info, color, content, *log_stream);
            }
        };

    private:
        void log(const std::string& info, const colored_stream::Color& color, const std::string& content, std::ostream& stream) {
            stream << '[' << colored_stream::ColoredStream{ color, info, ':', port } << "]= " << content << std::endl;
        };

        std::optional<std::ofstream> log_stream;
        const std::size_t port;
    };
}

void Server::run(const std::size_t port, const std::string& logFileName) {
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
      actions = getPossibleActions();
      actions_init_done = true;
    }
  }

  httplib::Server svr;

  Logger logger(logFileName, port);

  // register actions to server
  for (const auto &[name, action] : actions) {
    svr.Post("/getJSON", [&action = action, this, &port, &logger](const httplib::Request &req,
                                                  httplib::Response &res) {
      std::scoped_lock lock(this->actions_execution_mtx);
      logger.log("INFO", INFO_COLOR, req.body);
      res.set_header("Access-Control-Allow-Origin", "*");
      nlohmann::json gui_response;
      try {
        nlohmann::json json_request = nlohmann::json::parse(req.body);
        gui::Request gui_request = json_request.get<gui::Request>();
        action(gui_request, gui_response);
        auto gui_response_string = gui_response.dump();
        logger.log("INFO", INFO_COLOR, gui_response_string);
        res.set_content(gui_response_string, "text/plain");
        return;
      } catch (const std::exception &e) {
        gui_response = make_bad_response(e.what());
      } catch (...) {
        gui_response = make_bad_response("UNKNOWN");
      }
      auto gui_response_string = gui_response.dump();
      logger.log("ERROR", ERROR_COLOR, gui_response_string);
      res.set_content(gui_response_string, "text/plain");
    });
  }

  svr.listen("localhost", static_cast<int>(port));
}
} // namespace gui
