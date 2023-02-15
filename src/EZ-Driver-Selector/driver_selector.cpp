#include "main.h"
#include "pros/misc.h"

// Parameter increased, roll over
void increase_parameter() {
  if (parameter_place >= testing.size() - 1)
    parameter_place = 0;
  else
    parameter_place++;
  update_parameter();
}

// Parameter decreased, roll over
void decrease_parameter() {
  if (parameter_place <= 0)
    parameter_place = testing.size() - 1;
  else
    parameter_place--;
  update_parameter();
}

// Driver increase, roll over
void increase_driver() {
  if (current_driver >= drivers.size() - 1)
    current_driver = 0;
  else
    current_driver++;
  update_name();
  update_parameter();
}

// Driver decrease, roll over
void decrease_driver() {
  if (current_driver <= 0)
    current_driver = drivers.size() - 1;
  else
    current_driver--;
  update_name();
  update_parameter();
}

bool enabled = false;  // Is the gui active?
bool is_driver_selector_active() {
  return enabled;
}

// Main task
void driver_selector_task() {
  queue_clear_screen();  // Clear default stuff from screen
  cursor_placement = 1;
  update_name();  // Put the name on the screen, this also sets all the
  cursor_placement = 0;

  int count = 0;          // This is Used to make sure queue_iterate is being called every 50ms
  bool selected = false;  // Are we currently selected on the parameter?

  int timer = 0;               // Used for toggling the gui on/off
  bool was_triggered = false;  // Used for making sure button release happens on enable/disable

  while (true) {
    // If digital_y is held for 500ms and the controller is not connected to a competition,
    // the gui can be enabled and ran
    if (master.get_digital(DIGITAL_Y) && !pros::competition::is_connected()) {
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
          cursor_placement = 0;
        }
      }
      timer += 25;
    } else {
      timer = 0;
      was_triggered = false;
    }

    // If the GUI is enabled
    if (enabled) {
      // Disable gui if you connect to a competition
      if (pros::competition::is_connected())
        enabled = false;

      // Select button only works on the variable so you can modify it with arrows
      if (master.get_digital_new_press(DIGITAL_A)) {
        if (cursor_placement == 2) {
          selected = !selected;
          cursor = selected ? selected_cursor : unselected_cursor;
          update_cursor();
        }
      }

      // If not selected yet, D pad is for moving through the menu
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

      // Once selected, D pad is for modifying the variable of the parameter
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
