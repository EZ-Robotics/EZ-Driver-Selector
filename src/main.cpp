#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello PROS User!");

  pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  chassis.set_drive_brake(drivers[current_driver].brake_type);

  while (true) {
    switch (drivers[current_driver].drive_mode) {
      case tank:
        chassis.tank();
        break;
      case normal_split_arcade:
        chassis.arcade_standard(ez::SPLIT);
        break;
      case flipped_split_arcade:
        chassis.arcade_flipped(ez::SPLIT);
        break;
      case normal_single_arcade:
        chassis.arcade_standard(ez::SINGLE);
        break;
      case flipped_single_arcade:
        chassis.arcade_flipped(ez::SINGLE);
        break;
    }

    pros::delay(20);
  }
}

enum cata_state_e { DOWN,
                    NOT_DOWN };

cata_state_e CATA_STATE;

bool fire_cata = false;
int piston_index = 0;

void fire_piston(int input) {
  switch (input) {
    // No piston
    case 0:
      // boost piston off
      // damp driver off
      // damp overfill off
      break;

    // Boost
    case 1:
      // boost piston on
      // damp driver off
      // damp overfill off
      break;

    // Damp driver
    case 2:
      // boost piston off
      // damp driver on
      // damp overfill off
      break;

    // Damp overkill
    case 3:
      // boost piston off
      // damp driver on
      // damp overfill off
      break;

    default:
      // boost piston off
      // damp driver off
      // damp overfill off
      break;
  }
}

void fire_catapult(int piston_state = 0) {
  fire_cata = true;
  piston_index = piston_state;
}

bool is_cata_at_top() {
  return magic logic to check if the top limit switch was pressed sufficiently
}

bool is_arm_velocity_dropped() {
  return magic logic to detect the cata arm velocity dropped
}

bool is_arm_past_over_center() {
  return magic logic to make sure the piston wont over center the arm
}

void cata_pseudo_task() {
  int amount_to_move = 100;  // amount of ticks for motor to move to fire the cata and move the slip gear just enough that it doesn't reengage with the cata

  int output = 0;
  bool pids = false;
  int last_pos = 0;

  bool was_encoder_reset = false;
  bool piston_done = true;

  while (true) {
    // If cata just fired but hasn't come down yet
    if (fired_cata == true && bottom limit switch not pressed) {
      // Trigger piston
      if (piston != 0) {
        // If boost, check if piston is past over center and fire.  Stop holding once arm reaches top
        if (piston_index == boost) {
          if (is_arm_past_over_center()) {
            fire_piston(piston_index);
            piston_done = is_cata_at_top();
          }
        } 
        // If dampen, set the pistons to dampen
        // Check once the arm velocity drops, and gas the dampers then stop holding
        else {
          fire_piston(piston_index);
          if (is_arm_velocity_dropped()) {
            piston_done = true;
          }
        }
      }

      if (piston_done && is_cata_at_top()) {
        fired_cata = false;
        pids = false;
        CATA_STATE = NOT_DOWN;
      }
    }

    // Cata has fired and is coming down
    else if (bottom limit switch not pressed) {
      fire_piston(0);  // make sure pistons are off
      output = 127;
      CATA_STATE = NOT_DOWN;
    }

    // Cata is down
    else {
      // When cata is fired...
      if (fire_cata) {
        fire_piston(0);  // make sure pistons are off
        pids = true;
        last_pos = cata_motor.get_value();
        CATA_STATE = NOT_DOWN;
        was_encoder_reset = false;
      }
      // When cata is holding down...
      else {
        fire_piston(0);  // make sure pistons are off
        CATA_STATE = DOWN;
        output = 0;
        if (!was_encoder_reset) {
          // reset encoder
          was_encoder_reset = true;
        }
      }
    }

    motor = pids ? (amount_to_move - last_pos) * 2 : output;

    pros::delay(10);
  }
}

// Cata velocity
double dcata_velocity = 0.0;
double dlast_cata_velocity = 0.0;
double cata_velocity() { return dcata_velocity; }
double last_cata_velocity() { return dlast_cata_velocity; }

// Top limit switch timer
long top_limit_timer = 0;
long cata_vel_zero_timer = 0;
bool is_cata_at_top() {
  // Make sure arm velocity is 0 for some amount of time (ms) 
  // and the switch has been pressed for some amount of time (ms)
  if (top_limit_timer >= 100 && cata_vel_zero_timer >= 100) 
    return true;
  return false;
}
 
bool is_arm_velocity_dropped() {
  // This logic will probably have to be changed
  // We should run the damper a few times and graph the velocities throughout
  // run damper at different settings too
  // and using the data should figure out how to make this function more robust
  if (last_cata_velocity() - cata_velocity() >= 50) 
    return true;
  return false;
}
 
bool is_arm_past_over_center() {
  // literally no idea what this number should be
  // but this is the value boost would fire at
  if (fired_cata && amt > 200)
    return true;
  return false;
}

void cata_pseudo_task() {
  ...
  whatever was here stays here
  ...

  // Cata velocity stuff
  double raw_cata_velocity = 0.0;
  // How much the filter will condider previous information. 
  // A value of 1 will make the input the full value of the output, a value of 0 will make the input have no effect on the output.
  // This needs to be tuned
  const double kA = 0.5; 



  while (true) {
    ...
    whatever was here stays here
    ...

    dlast_cata_velocity = amt encoder;
    pros::delay(10):
    raw_cata_velocity = dlast_cata_velocity - amt encoder;
 
    // Below is options for both ema filtering and just raw values.  
    // If the raw values are smooth, then there's no reason for ema.  Try no ema first
    // dcata_velocity = raw_cata_velocity * kA + dcata_velocity * (1 - kA); // ema filtering
    dcata_velocity = raw_cata_velocity; // no ema filtering
    
    // Top limit timer
    if (top limit switch) {
      top_limit_timer+=10;
    } else {
      top_limit_timer = 0;
    }

    // Cata velocity 0 timer
    if (cata_velocity() == 0) {
      cata_vel_zero_timer+=10;
    } else {
      cata_vel_zero_timer = 0;
    }
  }