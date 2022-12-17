#pragma once

#include "api.h"
#include "pros/misc.h"

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

inline std::vector<controllerObjects> testing = {
    {"Joy", 1, 0, 4, {"Tank", "Spl Arc", "Fl Spl Arc", "Sing Arc", "Fl Sing Arc"}},
    {"Brake T", 1, 0, 2, {"Coast", "Brake", "Hold"}},
    {"L Curve", 0.1, 0.0, 10.0},
    {"R Curve", 0.1, 0.0, 10.0},
    {"Active Brake", 0.1, 0.0, 10.0},
};
inline int parameter_place = 0;

struct driver_values {
  std::string name;
  drive_mode_t drive_mode;
  pros::motor_brake_mode_e_t brake_type;
  double curve_l;
  double curve_r;
  double active_brake;
};

inline driver_values critter = {
    "Critter",
    normal_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    3.3,
    3.3,
    0.1};

inline driver_values charlie = {
    "Charlie",
    flipped_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    2.4,
    2.1,
    0.1};

inline driver_values ben = {
    "Ben",
    normal_split_arcade,
    pros::E_MOTOR_BRAKE_BRAKE,
    1.2,
    1.1,
    0.1};

inline driver_values jess = {
    "Jess",
    tank,
    pros::E_MOTOR_BRAKE_BRAKE,
    0,
    0,
    0.1};

inline std::vector<driver_values> drivers = {critter, charlie, ben, jess};
inline int current_driver = 0;

double current_value(driver_values *driver, int param);