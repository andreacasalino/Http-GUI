#include <HttpGui/Request.h>

namespace gui {
RequestField::RequestField() : RequestField("") {}

RequestField::RequestField(const std::string &content)
    : std::variant<std::string, std::vector<std::string>>(content) {}

RequestField::RequestField(const std::vector<std::string> &content)
    : std::variant<std::string, std::vector<std::string>>(content) {}

std::size_t RequestField::size() const {
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

const std::string &RequestField::operator*() const {
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

const std::string &RequestField::operator[](const std::size_t position) const {
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

bool RequestField::operator==(const RequestField &o) const {
  const auto this_size = this->size();
  if (this_size != o.size()) {
    return false;
  }
  for (std::size_t k = 0; k < this_size; ++k) {
    if ((*this)[k] != o[k]) {
      return false;
    }
  }
  return true;
}

void from_json(const nlohmann::json &j, RequestField &f) {
  if (j.is_string()) {
    f = std::move(RequestField{j.get<std::string>()});
    return;
  } else if (j.is_array()) {
    f = std::move(RequestField{j.get<std::vector<std::string>>()});
    return;
  }
  throw std::runtime_error{"Invalid RequestField"};
}

void RequestField::to_json(nlohmann::json &j) const {
  class Visitor {
  public:
    nlohmann::json &j;

    void operator()(const std::string &as_string) const { j = as_string; };

    void operator()(const std::vector<std::string> &as_vector) const {
      j = as_vector;
    };
  };
  Visitor visitor = Visitor{j};
  const std::variant<std::string, std::vector<std::string>> &as_variant = *this;
  std::visit(visitor, as_variant);
}

Request::Request(RequestContent &&fields) { this->fields = std::move(fields); }

bool Request::isNull() const { return this->fields.empty(); }

const RequestField *Request::operator[](const std::string &field_name) const {
  auto it = fields.find(field_name);
  if (it == fields.end()) {
    return nullptr;
  }
  return &it->second;
}

void from_json(const nlohmann::json &j, Request &r) {
  if (j.is_null()) {
    r = std::move(Request{});
    return;
  }

  if (j.is_string() && j.get<std::string>().empty()) {
    r = std::move(Request{});
    return;
  }

  if (j.is_array() || (!j.is_structured())) {
    throw std::runtime_error{"Invalid Field"};
  }
  RequestContent map;
  for (const auto &it : j.items()) {
    from_json(it.value(), map[it.key()]);
  }
  r = std::move(Request{std::move(map)});
}
} // namespace gui
