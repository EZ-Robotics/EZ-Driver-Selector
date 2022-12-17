#include "main.h"
#include "queue.hpp"

struct controllerObjects {
  std::string name;
  double increase_by;
  double min_size;
  double max_size;
  std::vector<std::string> names;
};

enum drive_mode_t { tank = 0,
                    normal_split_arcade = 1,
                    flipped_split_arcade = 2,
                    normal_single_arcade = 3,
                    flipped_single_arcade = 4 };

// Brake mode values: https://pros.cs.purdue.edu/v5/api/cpp/motors.html#pros-motor-brake-mode-e-t

std::vector<controllerObjects> testing = {
    {"Joy", 1, 0, 4, {"Tank", "Spl Arc", "Fl Spl Arc", "Sing Arc", "Fl Sing Arc"}},
    {"Brake T", 1, 0, 2, {"Coast", "Brake", "Hold"}},
    {"L Curve", 0.1, 0.0, 10.0},
    {"R Curve", 0.1, 0.0, 10.0},
    {"Active Brake", 0.1, 0.0, 10.0},
};
int parameter_place = 0;

struct driver_values {
  std::string name;
  drive_mode_t drive_mode;
  pros::motor_brake_mode_e_t brake_type;
  double curve_l;
  double curve_r;
  double active_brake;
};

driver_values jess = {
    "Jess",
    tank,
    pros::E_MOTOR_BRAKE_BRAKE,
    0,
    0,
    0.1};

driver_values charlie = {
    "Charlie",
    flipped_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    2.4,
    2.1,
    0.1};

std::vector<driver_values> drivers = {jess, charlie};
int default_driver = 0;

double current_value(driver_values *driver, int param) {
  switch (param) {
    case 0:
      return driver->drive_mode;
      break;
    case 1:
      return driver->brake_type;
      break;
    case 2:
      return driver->curve_l;
      break;
    case 3:
      return driver->curve_r;
      break;
    case 4:
      return driver->active_brake;
      break;
    default:
      return 0;
      break;
  }
  return 0;
}

int cursor_placement = 1;
void update_parameter() {
  std::string title = testing[parameter_place].name;
  std::string parameter;

  // Parameter is a number
  if (testing[parameter_place].names.size() == 0) {
    std::string num_text = std::to_string(current_value(&drivers[default_driver], parameter_place));
    parameter = num_text.substr(0, num_text.find(".") + 2);
  }

  // Parameter is a word
  else {
    parameter = testing[parameter_place].names[current_value(&drivers[default_driver], parameter_place)];
  }

  int gap = title.length() + parameter.length();

  queue_clear_line(2);
  if (cursor_placement == 1) queue_add(2, 0, "-");
  queue_add(2, 1, title);
  if (cursor_placement == 2)
    queue_add(2, title.length() + (19 - gap), "-" + parameter + "     ");
  else
    queue_add(2, title.length() + (20 - gap), parameter + "     ");
}

void update_name() {
  queue_clear_line(1);
  if (cursor_placement == 0) queue_add(1, 0, "-");
  queue_add(1, 1, drivers[default_driver].name);
  update_parameter();
}

void update_cursor() {
  queue_clear_screen();
  update_name();
  update_parameter();
}

void increase_driver() {
  if (default_driver >= drivers.size() - 1)
    default_driver = 0;
  else
    default_driver++;
  update_name();
}

void decrease_driver() {
  if (default_driver <= 0)
    default_driver = drivers.size() - 1;
  else
    default_driver--;
  update_name();
}

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

  int count = 0;

  queue_add(1, 1, drivers[default_driver].name);
  update_parameter();

  while (true) {
    /*
    if (master.get_digital_new_press(DIGITAL_A)) {
      if (cursor_placement >= 2)
        cursor_placement = 0;
      else
        cursor_placement++;
      update_cursor();
    } else if (master.get_digital_new_press(DIGITAL_Y)) {
      if (cursor_placement == 0)
        cursor_placement = 2;
      else
        cursor_placement--;
      update_cursor();
    }
    */

    // Display driver name
    if (master.get_digital_new_press(DIGITAL_UP)) {
      cursor_placement = cursor_placement >= 1 ? 0 : 1;
      update_cursor();
      // increase_driver();
    } else if (master.get_digital_new_press(DIGITAL_DOWN)) {
      cursor_placement = cursor_placement == 0 ? 1 : 0;
      update_cursor();
      // decrease_driver();
    }

    // Display parameters
    if (master.get_digital_new_press(DIGITAL_LEFT)) {
      cursor_placement = cursor_placement == 2 ? 1 : 2;
      update_cursor();
      // increase_parameter();
    } else if (master.get_digital_new_press(DIGITAL_RIGHT)) {
      cursor_placement = cursor_placement <= 1 ? 2 : 1;
      update_cursor();
      // decrease_parameter();
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
