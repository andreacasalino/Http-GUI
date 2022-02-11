#include <HttpGui/BadResponse.h>

namespace gui {
namespace {
static const std::string KEYWORD = "EXCEPTION";
}

nlohmann::json make_bad_response(const std::string &reason) {
  nlohmann::json result;
  result[KEYWORD] = reason;
  return result;
}
} // namespace gui
