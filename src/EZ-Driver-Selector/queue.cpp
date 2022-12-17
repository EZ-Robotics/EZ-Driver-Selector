#include "main.h"

std::vector<controllerScreenQueue> controller_queue;

void queue_add(std::uint8_t line, std::uint8_t col, std::string str) {
  controller_queue.push_back({line, col, str});
}

void queue_iterate() {
  if (controller_queue.empty())
    return;

  master.set_text(controller_queue[0].line, controller_queue[0].col, controller_queue[0].str);
  controller_queue.erase(controller_queue.begin());
}

void queue_clear_line(std::uint8_t line) {
  queue_add(line, 0, "                         ");
}

void queue_clear_screen() {
  queue_clear_line(0);
  queue_clear_line(1);
  queue_clear_line(2);
}