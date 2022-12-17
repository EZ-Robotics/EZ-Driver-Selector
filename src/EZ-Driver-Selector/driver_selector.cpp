#include "main.h"
#include "queue.hpp"
#include "text_gui.hpp"

void increase_parameter() {
  if (parameter_place >= testing.size() - 1)
    parameter_place = 0;
  else
    parameter_place++;
  update_parameter();
}

void decrease_parameter() {
  if (parameter_place <= 0)
    parameter_place = testing.size() - 1;
  else
    parameter_place--;
  update_parameter();
}

void driver_selector_task() {
  queue_clear_screen();
  update_name();
  update_parameter();

  int count = 0;
  bool selected = false;

  while (true) {
    if (master.get_digital_new_press(DIGITAL_A)) {
      if (cursor_placement == 2) {
        selected = !selected;
        cursor = selected ? selected_cursor : unselected_cursor;
        update_cursor();
      }
    }

    // If not selected, d pad moves selector
    if (!selected) {
      if (master.get_digital_new_press(DIGITAL_UP)) {
        cursor_placement = cursor_placement >= 1 ? 0 : 1;
        update_cursor();
        // increase_driver();
      } else if (master.get_digital_new_press(DIGITAL_DOWN)) {
        cursor_placement = cursor_placement == 0 ? 1 : 0;
        update_cursor();
      }

      else if (master.get_digital_new_press(DIGITAL_LEFT)) {
        if (cursor_placement >= 2) {
          cursor_placement = 1;
          update_cursor();
        } else {
          if (cursor_placement != 0)
            decrease_parameter();
          else
            decrease_driver();
        }
      }

      else if (master.get_digital_new_press(DIGITAL_RIGHT)) {
        if (cursor_placement == 1) {
          cursor_placement = 2;
          update_cursor();
        } else {
          if (cursor_placement != 0)
            increase_parameter();
          else
            increase_driver();
        }
      }
    }

    // If selected, d pad cycles through menu
    else {
      if (master.get_digital_new_press(DIGITAL_RIGHT)) {
        add_driver_default(&drivers[current_driver], parameter_place, testing[parameter_place].increase_by);
        update_parameter();
      } else if (master.get_digital_new_press(DIGITAL_LEFT)) {
        add_driver_default(&drivers[current_driver], parameter_place, -testing[parameter_place].increase_by);
        update_parameter();
      }
    }

    pros::delay(25);
    count++;

    if (count == 2) {
      queue_iterate();
      count = 0;
    }
  }
}
pros::Task DriverSelectorTask(driver_selector_task);
