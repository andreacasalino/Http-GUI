#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <vector>

namespace gui {
class Request {
public:
  class Field;

  /// mainly for testing
  Request(std::map<char, Field> &&fields);

  const std::map<char, Field> &getFields() const { return fields; }

private:
  std::map<char, Field> fields;
};

// void to_json(nlohmann::json &j, const Request &p);
// void from_json(const nlohmann::json &j, Request &p);

class Request::Field
    : protected std::variant<std::string, std::vector<std::string>> {
public:
  /// mainly for testing
  Field(const std::string &content);
  /// mainly for testing
  Field(const std::vector<std::string> &content);

  std::size_t size() const;

  const std::string &operator*() const;

  const std::string &operator[](const std::size_t position) const;
};

// void to_json(nlohmann::json &j, const Request::Field &p);
// void from_json(const nlohmann::json &j, Request::Field &p);
} // namespace gui
