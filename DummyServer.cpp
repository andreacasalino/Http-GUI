#include <HttpGui/Server.h>
#include <iostream>

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
          nlohmann::json new_person;
          new_person["name"] = **name;
          new_person["surname"] = **surname;
          persons_ptr->push_back(new_person);
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

  std::vector<nlohmann::json> persons;
};

int main() {
  ServerSample server;
  server.run(52000, "log_file_additional");

  return EXIT_SUCCESS;
}
