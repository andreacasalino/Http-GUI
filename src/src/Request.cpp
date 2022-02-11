#include <HttpGui/Request.h>

namespace gui {
Request::Field::Field(const std::string &content)
    : std::variant<std::string, std::vector<std::string>>(content) {}

Request::Field::Field(const std::vector<std::string> &content)
    : std::variant<std::string, std::vector<std::string>>(content) {}

std::size_t Request::Field::size() const {
  class Visitor {
  public:
    mutable std::size_t result = 0;

    void operator()(const std::string &as_string) const { result = 1; };

    void operator()(const std::vector<std::string> &as_vector) const {
      result = as_vector.size();
    };
  };
  Visitor visitor;
  const std::variant<std::string, std::vector<std::string>> &as_variant = *this;
  std::visit(visitor, as_variant);
  return visitor.result;
}

const std::string &Request::Field::operator*() const {
  struct Visitor {
    mutable const std::string *result = nullptr;

    void operator()(const std::string &as_string) const {
      result = &as_string;
    };

    void operator()(const std::vector<std::string> &as_vector) const {
      result = &as_vector.front();
    };
  };

  Visitor visitor;
  const std::variant<std::string, std::vector<std::string>> &as_variant = *this;
  std::visit(visitor, as_variant);
  return *visitor.result;
}

const std::string &
Request::Field::operator[](const std::size_t position) const {
  struct Visitor {
    const std::size_t &position;
    mutable const std::string *result = nullptr;

    void operator()(const std::string &as_string) const {
      result = &as_string;
    };

    void operator()(const std::vector<std::string> &as_vector) const {
      result = &as_vector[this->position];
    };
  };

  Visitor visitor = Visitor{position};
  const std::variant<std::string, std::vector<std::string>> &as_variant = *this;
  std::visit(visitor, as_variant);
  return *visitor.result;
}

Request::Request(std::map<char, Field> &&fields) {
  this->fields = std::move(fields);
}
} // namespace gui
