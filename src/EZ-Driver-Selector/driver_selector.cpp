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

struct driver_values {
  std::string name = "Jess:";
  drive_mode_t drive_mode;
  pros::motor_brake_mode_e_t brake_type;
  double curve_l;
  double curve_r;
  double active_brake;
};

driver_values jess = {
    "jess",
    tank,
    pros::E_MOTOR_BRAKE_BRAKE,
    0,
    0,
    0.1};

driver_values charlie = {
    "charlie",
    flipped_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    2.4,
    2.1,
    0.1};

std::vector<driver_values> drivers = {jess, charlie};
int default_driver = 0;

void driver_selector_task() {
  queue_clear_screen();

  int selected_line = 1;
  // queue_add(selected_line, 0, "|");

  int count = 0;

  queue_add(1, 0, " " + drivers[default_driver].name);

  while (true) {
    /**
    // Switch up/down between cycling driver and parameters
    if (master.get_digital_new_press(UP_DOWN)) {
      queue_add(selected_line, 0, " ");
      selected_line = selected_line == 1 ? 2 : 1;
      queue_add(selected_line, 0, "|");
    }

    // Cycle between types in parameter
    if (master.get_digital_new_press(CYCLE_LEFT_RIGHT)) {
      if (selected_line == 2) {
      }
    }
    */

    pros::delay(25);
    count++;

    if (count == 2) {
      queue_iterate();
      count = 0;
    }
  }
}
pros::Task DriverSelectorTask(driver_selector_task);
