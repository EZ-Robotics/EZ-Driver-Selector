#pragma once

#include "api.h"
#include "pros/misc.h"

#define UP_DOWN pros::E_CONTROLLER_DIGITAL_L1
#define LEFT_RIGHT pros::E_CONTROLLER_DIGITAL_R1
#define CYCLE_LEFT_RIGHT pros::E_CONTROLLER_DIGITAL_R2

inline pros::Controller master(pros::E_CONTROLLER_MASTER);
