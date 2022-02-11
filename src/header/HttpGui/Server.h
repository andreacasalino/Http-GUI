#include <HttpGui/Request.h>
#include <functional>
#include <mutex>
#include <set>

namespace gui {
using Response = nlohmann::json;
using Action = std::function<void(const Request &, Response &)>;
using Actions = std::unordered_map<std::string, Action>;

class Server {
public:
  void run(const std::size_t port, const std::string& logFileName = "");

protected:
  Server() = default;

  virtual Actions getPOSTActions() = 0;
  virtual Actions getGETActions() = 0;

  add default stop action

private:
  std::mutex consumed_ports_mtx;
  std::set<std::size_t> consumed_ports;

  std::mutex actions_mtx;
  bool actions_init_done = false;
  Actions actions;

  std::mutex actions_execution_mtx;
};
} // namespace gui
