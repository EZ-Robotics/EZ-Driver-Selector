#include "main.h"

std::vector<std::string> testing = {"hi"};

// left curve
// right curve
// active brake
// brake type
// drive type

struct controllerScreenQueue {
  std::uint8_t line;
  std::uint8_t col;
  std::string str;
};
std::vector<controllerScreenQueue> queue;

void queue_add(std::uint8_t line, std::uint8_t col, std::string str) {
  queue.push_back({line, col, str});
}

void queue_run() {
  if (queue.empty())
    return;

  master.set_text(queue[0].line, queue[0].col, queue[0].str);
  queue.erase(queue.begin());
}

void queue_clear_line(std::uint8_t line) {
  queue_add(line, 0, "                  ");
}

void queue_clear_screen() {
  queue_clear_line(0);
  queue_clear_line(1);
  queue_clear_line(2);
}

int prev_line = -1;
void set_text(std::uint8_t line, std::uint8_t col, const std::string &str) {
  if (prev_line != -1)
    queue_clear_line(prev_line);
  prev_line = line;
  queue_add(line, col, str);
}

void driver_selector_task() {
  queue_clear_screen();

  int selected_line = 1;

  while (true) {

    if (master.get_digital_new_press(DIGITAL_L1)) {
      selected_line = selected_line == 1 ? 2 : 1;
      queue_add(selected_line == 1 ? 2 : 1, 0, " ");
      queue_add(selected_line, 0, "|");
    }

    pros::delay(50);

    queue_run();
  }
}
pros::Task DriverSelectorTask(driver_selector_task);
