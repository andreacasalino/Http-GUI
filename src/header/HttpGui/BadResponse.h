#pragma once

#include <nlohmann/json.hpp>

namespace gui {
nlohmann::json make_bad_response(const std::string &reason);
} // namespace gui
