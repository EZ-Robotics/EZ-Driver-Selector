#include "main.h"

namespace driver {
struct controllerScreenQueue {
  std::uint8_t line;
  std::uint8_t col;
  std::string str;
};

void queue_add(std::uint8_t line, std::uint8_t col, std::string str);
void queue_iterate();
void queue_clear_line(std::uint8_t line);
void queue_clear_screen();
}  // namespace driver