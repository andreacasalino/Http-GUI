#include "ServerLogger.h"
#include "StringMerger.h"
#include "httplib.h"
#include <HttpGui/BadResponse.h>
#include <HttpGui/Definitions.h>
#include <HttpGui/Server.h>

namespace gui {
namespace {
httplib::Server::Handler
make_action_for_server_registering(std::mutex &action_mtx, ServerLogger &logger,
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

  ServerLogger logger_POST_actions(logFileName, port, "[POST]");
  ServerLogger logger_GET_actions(logFileName, port, "[GET]");

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
