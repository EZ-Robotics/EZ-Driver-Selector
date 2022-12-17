#include "main.h"

// This is used so we treat the struct driver_values more like an array or a vector
// when added is 0, this just returns what the current value is.  if added is anything else,
// the default driver parameter gets modified.
double add_driver_default(driver_values *driver, int param, double added) {
  // Figure out our mins and maxes for our value
  double min = testing[parameter_place].min_size;
  double max = testing[parameter_place].max_size;
  double mod = 0;

  switch (param) {
    // Drive type
    case 0:
      mod = driver->drive_mode + added;                 // Add increment
      mod = mod > max ? min : (mod < min ? max : mod);  // Make sure it's within bounds
      driver->drive_mode = (drive_mode_t)mod;           // Set it as default
      break;

    // Brake type
    case 1:
      mod = driver->brake_type + added;                      // Add increment
      mod = mod > max ? min : (mod < min ? max : mod);       // Make sure it's within bounds
      driver->brake_type = (pros::motor_brake_mode_e_t)mod;  // Set it as default
      break;

    // Left curve
    case 2:
      mod = driver->curve_l + added;                    // Add increment
      mod = mod > max ? min : (mod < min ? max : mod);  // Make sure it's within bounds
      driver->curve_l = mod;                            // Set it as default
      break;

    // Right curve
    case 3:
      mod = driver->curve_r + added;                    // Add increment
      mod = mod > max ? min : (mod < min ? max : mod);  // Make sure it's within bounds
      driver->curve_r = mod;                            // Set it as default
      break;

    // Active brake
    case 4:
      mod = driver->active_brake + added;               // Add increment
      mod = mod > max ? min : (mod < min ? max : mod);  // Make sure it's within bounds
      driver->active_brake = mod;                       // Set it as default
      break;
    default:
      return 0;
      break;
  }

  // Update new constants using ez-template
  chassis.set_drive_brake(driver->brake_type);
  chassis.set_curve_default(driver->curve_l, driver->curve_r);
  chassis.set_active_brake(driver->active_brake);

  return mod;  // Return final value
}

// Uses above function and returns the current value
double current_value(driver_values *driver, int param) {
  return add_driver_default(driver, param, 0);
}

// Updates the parameter and title line visually
void update_parameter() {
  std::string title = testing[parameter_place].name;
  std::string parameter;

  // Parameter is a number
  if (testing[parameter_place].names.size() == 0) {
    std::string num_text = std::to_string(current_value(&drivers[current_driver], parameter_place));
    parameter = num_text.substr(0, num_text.find(".") + 3);
  }

  // Parameter is a word
  else {
    parameter = testing[parameter_place].names[current_value(&drivers[current_driver], parameter_place)];
  }

  // Gap between title and parameter
  int gap = title.length() + parameter.length();

  // Print text and cursor if applicable
  queue_clear_line(2);
  if (cursor_placement == 1) queue_add(2, 0, cursor);
  queue_add(2, 1, title);
  if (cursor_placement == 2)
    queue_add(2, title.length() + (18 - gap), cursor + parameter + "     ");
  else
    queue_add(2, title.length() + (19 - gap), parameter + "     ");
}

// Update name visually
void update_name() {
  queue_clear_line(1);
  // Print name and cursor if applicable
  if (cursor_placement == 0) queue_add(1, 0, cursor);
  queue_add(1, 1, drivers[current_driver].name);

  // Update the defaults to this drivers defaults
  current_value(&drivers[current_driver], 0);
  current_value(&drivers[current_driver], 1);
  current_value(&drivers[current_driver], 2);
  current_value(&drivers[current_driver], 3);
  current_value(&drivers[current_driver], 4);
}

// Update cursor visually
void update_cursor() {
  queue_clear_line(1);
  queue_clear_line(2);
  update_name();
  update_parameter();
}

// Update battery visually
void update_battery() {
  queue_add(0, 1, std::to_string(battery_percentage) + "%     ");
}
