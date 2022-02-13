#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <HttpGui/Request.h>

TEST_CASE("RequestField from proto", "[request][field][io]") {
  SECTION("string") {
    const std::string field_content = "hello";

    nlohmann::json field_json = field_content;
    gui::RequestField field = field_json.get<gui::RequestField>();

    CHECK(*field == field_content);
    CHECK(field.size() == 1);
  }

  SECTION("string vector") {
    const std::vector<std::string> field_content =
        std::vector<std::string>{"hello", "hello", "world", "world"};

    nlohmann::json field_json = field_content;
    gui::RequestField field = field_json.get<gui::RequestField>();

    CHECK(*field == field_content.front());
    REQUIRE(field.size() == field_content.size());
    for (std::size_t k = 0; k < field_content.size(); ++k) {
      CHECK(field[k] == field_content[k]);
    }
  }
}

TEST_CASE("Request from proto", "[request][io]") {
  SECTION("null request") {
    std::string null_string = "null";
    auto null_json = nlohmann::json::parse(null_string);
    gui::Request request = null_json.get<gui::Request>();
    CHECK(request.isNull());
  }

  SECTION("Positive tests") {
    const auto map = GENERATE(
        gui::RequestContent{{"fieldA", gui::RequestField{"valueA"}},
                            {"fieldB", gui::RequestField{"valueB"}},
                            {"fieldC", gui::RequestField{"valueC"}}},

        gui::RequestContent{
            {"fieldA", gui::RequestField{"valueA"}},
            {"fieldB",
             gui::RequestField{std::vector<std::string>{"valueB1", "valueB2"}}},
            {"fieldC", gui::RequestField{"valueC"}}},

        gui::RequestContent{
            {"fieldA", gui::RequestField{"valueA"}},
            {"fieldB",
             gui::RequestField{std::vector<std::string>{"valueB1", "valueB2"}}},
            {"fieldC", gui::RequestField{std::vector<std::string>{}}}});

    nlohmann::json as_json;
    for (const auto &[name, value] : map) {
      value.to_json(as_json[name]);
    }

    gui::Request request = as_json.get<gui::Request>();

    CHECK(request.getContent() == map);
  }

  SECTION("Negative tests") {
    nlohmann::json as_json;
    as_json.push_back("val1");
    as_json.push_back("val2");
    as_json.push_back("val3");

    CHECK_THROWS(as_json.get<gui::Request>());
  }
}
