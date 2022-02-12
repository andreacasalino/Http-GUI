#include <HttpGui/BadResponse.h>
#include <HttpGui/Definitions.h>

namespace gui {
nlohmann::json make_bad_response(const std::string &reason) {
  nlohmann::json result;
  result[EXCEPTION_RESPONSE_KEYWORD] = reason;
  return result;
}
} // namespace gui
