#include "main.h"

double add_driver_default(driver_values *driver, int param, double added) {
  double min = testing[parameter_place].min_size;
  double max = testing[parameter_place].max_size;
  double mod = 0;

  switch (param) {
    case 0:
      mod = driver->drive_mode + added;
      mod = mod > max ? min : (mod < min ? max : mod);
      driver->drive_mode = (drive_mode_t)mod;
      break;
    case 1:
      mod = driver->brake_type + added;
      mod = mod > max ? min : (mod < min ? max : mod);
      driver->brake_type = (pros::motor_brake_mode_e_t)mod;
      break;
    case 2:
      mod = driver->curve_l + added;
      mod = mod > max ? min : (mod < min ? max : mod);
      driver->curve_l = mod;
      break;
    case 3:
      mod = driver->curve_r + added;
      mod = mod > max ? min : (mod < min ? max : mod);
      driver->curve_r = mod;
      break;
    case 4:
      mod = driver->active_brake + added;
      mod = mod > max ? min : (mod < min ? max : mod);
      driver->active_brake = mod;
      break;
    default:
      return 0;
      break;
  }
  chassis.set_drive_brake(driver->brake_type);
  chassis.set_curve_default(driver->curve_l, driver->curve_r);
  chassis.set_active_brake(driver->active_brake);
  return mod;
}

double current_value(driver_values *driver, int param) {
  return add_driver_default(driver, param, 0);
}

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

  int gap = title.length() + parameter.length();

  queue_clear_line(2);
  if (cursor_placement == 1) queue_add(2, 0, cursor);
  queue_add(2, 1, title);
  if (cursor_placement == 2)
    queue_add(2, title.length() + (18 - gap), cursor + parameter + "     ");
  else
    queue_add(2, title.length() + (19 - gap), parameter + "     ");
}

void update_name() {
  queue_clear_line(1);
  if (cursor_placement == 0) queue_add(1, 0, cursor);
  queue_add(1, 1, drivers[current_driver].name);
  current_value(&drivers[current_driver], 0);
  current_value(&drivers[current_driver], 1);
  current_value(&drivers[current_driver], 2);
  current_value(&drivers[current_driver], 3);
  current_value(&drivers[current_driver], 4);
  // update_parameter();
}

void update_cursor() {
  queue_clear_line(1);
  queue_clear_line(2);
  update_name();
  update_parameter();
}

void update_battery() {
  queue_add(0, 1, std::to_string(battery_percentage) + "%     ");
}
