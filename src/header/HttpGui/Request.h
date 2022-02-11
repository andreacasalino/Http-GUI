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
  RequestField();

  /// mainly for testing
  RequestField(const std::string &content);
  /// mainly for testing
  RequestField(const std::vector<std::string> &content);

  std::size_t size() const;

  const std::string &operator*() const;

  const std::string &operator[](const std::size_t position) const;

  bool operator==(const RequestField &o) const;

  // mainly for testing
  void to_json(nlohmann::json &j) const;
};

void from_json(const nlohmann::json &j, RequestField &f);

using RequestContent = std::unordered_map<std::string, RequestField>;

class Request {
public:
  Request() = default;

  /// mainly for testing
  Request(RequestContent &&fields);

  const RequestContent &getContent() const { return fields; }

  bool isNull() const;

private:
  RequestContent fields;
};

void from_json(const nlohmann::json &j, Request &r);
} // namespace gui
