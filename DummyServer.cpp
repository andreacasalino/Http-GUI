#include <HttpGui/Server.h>
#include <iostream>

struct Person {
  std::string name;
  std::string surname;
};

void to_json(nlohmann::json &recipient, const Person &person) {
  recipient["name"] = person.name;
  recipient["surname"] = person.surname;
}

class ServerSample : public gui::Server {
public:
  ServerSample() = default;

protected:
  gui::Actions getPOSTActions() override {
    gui::Actions actions;
    auto *persons_ptr = &persons;
    actions.emplace(
        "Add", [persons_ptr](const gui::Request &req, gui::Response &resp) {
          auto *name = req["name"];
          if (nullptr == name) {
            throw std::runtime_error{"name not specified"};
          }
          auto *surname = req["surname"];
          if (nullptr == surname) {
            throw std::runtime_error{"surname not specified"};
          }
          persons_ptr->push_back(Person{**name, **surname});
          // resp assumed null as never differently set
        });
    return actions;
  }

  gui::Actions getGETActions() override {
    gui::Actions actions;
    auto *persons_ptr = &persons;
    actions.emplace(
        "Get", [persons_ptr](const gui::Request &req, gui::Response &resp) {
          resp = *persons_ptr;
        });
    return actions;
  }

  std::vector<Person> persons;
};

int main() {
  ServerSample server;
  server.run(52000, "log_file_additional");

  return EXIT_SUCCESS;
}
