#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace gui {
class RequestField
    : protected std::variant<std::string, std::vector<std::string>> {
public:
  /// mainly for testing
  RequestField(const std::string &content);
  /// mainly for testing
  RequestField(const std::vector<std::string> &content);

  std::size_t size() const;

  const std::string &operator*() const;

  const std::string &operator[](const std::size_t position) const;
};

void from_json(const nlohmann::json &j, RequestField &f);

using RequestMap = std::unordered_map<std::string, RequestField>;

class Request {
public:
  /// mainly for testing
  Request(RequestMap &&fields);

  const RequestMap &getFields() const { return fields; }

private:
  RequestMap fields;
};

void from_json(const nlohmann::json &j, Request &r);
} // namespace gui
