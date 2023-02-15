#pragma once

#include "api.h"
#include "pros/misc.h"
#include "pros/motors.h"

bool is_driver_selector_active();

inline int battery_percentage = 0;

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

enum controller_t {
  CONTROLLER_DIGITAL_L1 = 0,
  CONTROLLER_DIGITAL_L2 = 1,
  CONTROLLER_DIGITAL_R1 = 2,
  CONTROLLER_DIGITAL_R2 = 3,
  CONTROLLER_DIGITAL_UP = 4,
  CONTROLLER_DIGITAL_DOWN = 5,
  CONTROLLER_DIGITAL_LEFT = 6,
  CONTROLLER_DIGITAL_RIGHT = 7,
  CONTROLLER_DIGITAL_X = 8,
  CONTROLLER_DIGITAL_B = 9,
  CONTROLLER_DIGITAL_Y = 10,
  CONTROLLER_DIGITAL_A = 11
};

inline pros::controller_digital_e_t INTAKE_BUTTON;
inline pros::controller_digital_e_t OUTAKE_BUTTON;
inline pros::controller_digital_e_t NORMAL_BUTTON;
inline pros::controller_digital_e_t DAMP_BUTTON;
inline pros::controller_digital_e_t BOOST_BUTTON;
inline pros::controller_digital_e_t INFAIL_BUTTON;

inline std::vector<controllerObjects> testing = {
    {"Joy", 1, 0, 4, {"Tank", "Spl Arc", "Fl Spl Arc", "Sing Arc", "Fl Sing Arc"}},
    {"Brake T", 1, 0, 2, {"Coast", "Brake", "Hold"}},
    {"L Curve", 0.2, 0.0, 5.0},
    {"R Curve", 0.2, 0.0, 5.0},
    {"Active Brake", 0.05, 0.0, 3.0},
    {"Intake", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
    {"Outake", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
    {"Normal", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
    {"Damp", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
    {"Boost", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
    {"InFail", 1, 0, 11, {"L1", "L2", "R1", "R2", "U", "D", "L", "R", "X", "B", "Y", "A"}},
};
inline int parameter_place = 0;

struct driver_values {
  std::string name;
  drive_mode_t drive_mode;
  pros::motor_brake_mode_e_t brake_type;
  double curve_l;
  double curve_r;
  double active_brake;
  controller_t intake_button;
  controller_t outake_button;
  controller_t normal_button;
  controller_t damp_button;
  controller_t boost_button;
  controller_t infail_button;
};

inline driver_values critter = {
    "Critter",
    normal_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    2,
    3,
    0.05,
    CONTROLLER_DIGITAL_R1,
    CONTROLLER_DIGITAL_R2,
    CONTROLLER_DIGITAL_B,
    CONTROLLER_DIGITAL_DOWN,
    CONTROLLER_DIGITAL_A,
    CONTROLLER_DIGITAL_LEFT,
};

inline driver_values charlie = {
    "Charlie",
    flipped_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    0.6,
    0.4,
    0.15,
    CONTROLLER_DIGITAL_R1,
    CONTROLLER_DIGITAL_R2,
    CONTROLLER_DIGITAL_L1,
    CONTROLLER_DIGITAL_L2,
    CONTROLLER_DIGITAL_DOWN,
    CONTROLLER_DIGITAL_LEFT,
};

inline driver_values ben = {
    "Ben",
    normal_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    0.0,
    0.0,
    0.05,
    CONTROLLER_DIGITAL_L1,
    CONTROLLER_DIGITAL_L2,
    CONTROLLER_DIGITAL_R1,
    CONTROLLER_DIGITAL_R2,
    CONTROLLER_DIGITAL_B,
    CONTROLLER_DIGITAL_LEFT,
};

inline driver_values jess = {
    "Jess",
    tank,
    pros::E_MOTOR_BRAKE_BRAKE,
    0,
    0,
    0.15,
    CONTROLLER_DIGITAL_L1,
    CONTROLLER_DIGITAL_L2,
    CONTROLLER_DIGITAL_R1,
    CONTROLLER_DIGITAL_R2,
    CONTROLLER_DIGITAL_B,
    CONTROLLER_DIGITAL_LEFT,
};

inline int current_driver = 0;

double current_value(driver_values *driver, int param);
