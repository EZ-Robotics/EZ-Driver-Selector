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

void increase_driver() {
  if (current_driver >= drivers.size() - 1)
    current_driver = 0;
  else
    current_driver++;
  update_name();
  update_parameter();
}

void decrease_driver() {
  if (current_driver <= 0)
    current_driver = drivers.size() - 1;
  else
    current_driver--;
  update_name();
  update_parameter();
}

void driver_selector_task() {
  queue_clear_screen();
  update_name();

  int count = 0;
  bool selected = false;

  bool enabled = false;

  int timer = 0;
  int was_pressed = 0;
  bool was_triggered = false;

  while (true) {
    if (master.get_digital(DIGITAL_A) && !pros::competition::is_connected()) {
      if (timer > 500 && !was_triggered) {
        enabled = !enabled;
        was_triggered = true;
        timer = 0;
        if (enabled) {
          update_cursor();
        } else {
          queue_clear_line(2);
          cursor_placement = 1;
          update_name();
          update_battery();
        }
      }
      timer += 25;

    } else {
      timer = 0;
      was_triggered = false;
    }

    if (enabled) {
      was_pressed = master.get_digital(DIGITAL_A);
      if (master.get_digital(DIGITAL_A) && !was_pressed) {
        if (cursor_placement == 2) {
          selected = !selected;
          cursor = selected ? selected_cursor : unselected_cursor;
          update_cursor();
        }
      }

      if (!selected) {
        if (master.get_digital_new_press(DIGITAL_UP)) {
          cursor_placement = cursor_placement >= 1 ? 0 : 1;
          update_cursor();
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

      else {
        if (master.get_digital_new_press(DIGITAL_RIGHT)) {
          add_driver_default(&drivers[current_driver], parameter_place, testing[parameter_place].increase_by);
          update_parameter();
        } else if (master.get_digital_new_press(DIGITAL_LEFT)) {
          add_driver_default(&drivers[current_driver], parameter_place, -testing[parameter_place].increase_by);
          update_parameter();
        }
      }
    }
    // Update battery percentage
    if (battery_percentage != pros::battery::get_capacity()) {
      battery_percentage = pros::battery::get_capacity();
      update_battery();
    }

    count++;
    pros::delay(25);

    // Update controller every 50ms
    if (count >= 2) {
      queue_iterate();
      count = 0;
    }
  }
}
pros::Task DriverSelectorTask(driver_selector_task);
