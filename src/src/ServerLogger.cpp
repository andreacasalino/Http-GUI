#include "ServerLogger.h"

namespace gui {
static const colored_stream::Color INFO_COLOR =
    colored_stream::ClassicColor::GREEN;
static const std::string INFO_DESCR = "INFO";

static const colored_stream::Color ERROR_COLOR =
    colored_stream::ClassicColor::RED;
static const std::string ERROR_DESCR = "ERROR";

ServerLogger::ServerLogger(const std::string &fileName, const std::size_t port,
                           const std::string &preamble)
    : preamble(preamble), port(port) {
  if (!fileName.empty()) {
    log_stream = std::make_optional<std::ofstream>(fileName);
    if (!log_stream->is_open()) {
      log_stream.reset();
    }
  }
};

void ServerLogger::log_info(const std::string &message) {
  log(message, INFO_DESCR, INFO_COLOR);
};

void ServerLogger::log_error(const std::string &message) {
  log(message, ERROR_DESCR, ERROR_COLOR);
};

void ServerLogger::log(const std::string &message, const std::string &level,
                       const colored_stream::Color &level_color) {
  log(message, level, level_color, std::cout);
  if (log_stream) {
    log(message, level, level_color, *log_stream);
  }
};

void ServerLogger::log(const std::string &message, const std::string &level,
                       const colored_stream::Color &level_color,
                       std::ostream &stream) {
  stream << '[' << colored_stream::ColoredStream{level_color, port} << ']'
         << '[' << colored_stream::ColoredStream{level_color, level} << ']'
         << preamble << "= " << message << std::endl;
};
} // namespace gui
