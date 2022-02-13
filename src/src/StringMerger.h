#pragma once

#include <sstream>
#include <string>

namespace gui {
class StringMerger {
public:
  template <typename T1, typename T2, typename... Args>
  static std::string merge(const T1 &first, const T2 &second, Args... args) {
    std::stringstream stream;
    merge_(stream, first, second, args...);
    return stream.str();
  }

private:
  template <typename T, typename... Args>
  static void merge_(std::stringstream &stream, const T &element,
                     Args... args) {
    merge_(stream, element);
    merge_(stream, args...);
  }

  template <typename T>
  static void merge_(std::stringstream &stream, const T &element) {
    stream << element;
  }
};
} // namespace gui
