#include "queue.hpp"

#include "main.h"

using namespace driver;

std::vector<controllerScreenQueue> queue;

void queue_add(std::uint8_t line, std::uint8_t col, std::string str) {
  queue.push_back({line, col, str});
}

void queue_iterate() {
  if (queue.empty())
    return;

  master.set_text(queue[0].line, queue[0].col, queue[0].str);
  queue.erase(queue.begin());
}

void queue_clear_line(std::uint8_t line) {
  driver::queue_add(line, 0, "                         ");
}

void queue_clear_screen() {
  driver::queue_clear_line(0);
  driver::queue_clear_line(1);
  driver::queue_clear_line(2);
}