#include "httplib.h"
#include <HttpGui/BadResponse.h>
#include <HttpGui/Server.h>
#include <sstream>

namespace gui {
void Server::run(const std::size_t port) {
  {
    std::scoped_lock(consumed_ports_mtx);
    auto it_port = consumed_ports.find(port);
    if (it_port != consumed_ports.end()) {
      std::stringstream message;
      message << std::to_string(port) << " is an already in use port";
      throw std::runtime_error{message.str()};
    }
    consumed_ports.emplace(port);
  }

  {
    std::scoped_lock(actions_mtx);
    if (!actions_init_done) {
      actions = getPossibleActions();
      actions_init_done = true;
    }
  }

  httplib::Server svr;

  // register actions to server
  for (const auto &[name, action] : actions) {
    svr.Post("/getJSON", [&action = action, this](const httplib::Request &req,
                                                  httplib::Response &res) {
      std::scoped_lock(this->actions_execution_mtx);
      // TODO log request using colors
      res.set_header("Access-Control-Allow-Origin", "*");
      nlohmann::json gui_response;
      try {
        gui::Request gui_request = ; // TODO parse from req.body
        action(gui_request, gui_response);
        // TODO log response with green color
      } catch (const std::exception &e) {
        gui_response = make_bad_response(e.what());
        // TODO log response with red color
      } catch (...) {
        gui_response = make_bad_response("UNKNOWN");
        // TODO log response with red color
      }
      res.set_content(gui_response.dump(), "text/plain");
    });
  }

  svr.listen("localhost", port);
}
} // namespace gui
