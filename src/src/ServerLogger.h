#pragma once

#include <ColoredStream/ColoredStream.h>
#include <fstream>
#include <iostream>
#include <optional>

namespace gui {
class ServerLogger {
public:
  ServerLogger(const std::string &fileName, const std::size_t port,
               const std::string &preamble);

  void log_info(const std::string &message);

  void log_error(const std::string &message);

private:
  void log(const std::string &message, const std::string &level,
           const colored_stream::Color &level_color);

  void log(const std::string &message, const std::string &level,
           const colored_stream::Color &level_color, std::ostream &stream);

  const std::string preamble;
  std::optional<std::ofstream> log_stream;
  const std::size_t port;
};
} // namespace gui
