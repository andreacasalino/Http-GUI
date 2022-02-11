#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <HttpGui/Request.h>

TEST_CASE("Request::Field size", "[request][field]") {

  SECTION("single string") { CHECK(gui::Request::Field{"hello"}.size() == 1); }

  SECTION("string vector") {
    const auto values =
        GENERATE(std::vector<std::string>{}, std::vector<std::string>{"hello"},
                 std::vector<std::string>{"hello", "world"});

    gui::Request::Field field(values);
    CHECK(field.size() == values.size());
  }
}

TEST_CASE("Request::Field operator*", "[request][field]") {

  SECTION("single string") {
    const std::string value = "hello";

    gui::Request::Field field(value);
    CHECK(*field == value);
  }

  SECTION("string vector") {
    const auto values =
        GENERATE(std::vector<std::string>{"hello"},
                 std::vector<std::string>{"hello", "world"},
                 std::vector<std::string>{"hello", "world", "hello", "world"});

    gui::Request::Field field(values);
    CHECK(*field == values.front());
  }
}

TEST_CASE("Request::Field operator[]", "[request][field]") {

  const auto position = GENERATE(0, 1, 2);

  SECTION("single string") {
    const std::string value = "hello";

    gui::Request::Field field(value);
    CHECK(field[position] == value);
  }

  SECTION("string vector") {
    const std::vector<std::string> values =
        std::vector<std::string>{"hello", "world", "hello", "world"};

    gui::Request::Field field(values);
    CHECK(field[position] == values[position]);
  }
}
