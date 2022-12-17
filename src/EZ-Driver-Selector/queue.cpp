#include "main.h"

/*
The controller can only be updated every 50ms.  Between clearing lines and writing, you need
50ms between each thing.  Instead of being careful and making sure there's adequate time
between each function call, this file is a queue system that handles it for us.  If it gets
overloaded, it'll cycle through until it's done.
*/

// Vector for the queue
std::vector<controllerScreenQueue> controller_queue;

// Add to queue
void queue_add(std::uint8_t line, std::uint8_t col, std::string str) {
  controller_queue.push_back({line, col, str});
}

// Iterate through queue
void queue_iterate() {
  if (controller_queue.empty())
    return;  // If queue is empty, leave this function

  // Set most current queue to screen
  master.set_text(controller_queue[0].line, controller_queue[0].col, controller_queue[0].str);
  controller_queue.erase(controller_queue.begin());  // Erase the queue from the vector
}

// Clear lines but it queues
void queue_clear_line(std::uint8_t line) {
  queue_add(line, 0, "                         ");
}

// Clear screen but it queues
void queue_clear_screen() {
  queue_clear_line(0);
  queue_clear_line(1);
  queue_clear_line(2);
}