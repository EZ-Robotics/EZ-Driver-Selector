#include "api.h"
#include "driver_selector.hpp"

inline std::string unselected_cursor = "#";
inline std::string selected_cursor = "&";
inline std::string cursor = unselected_cursor;

inline int cursor_placement = 0;

double add_driver_default(driver_values *driver, int param, double added);
pros::controller_digital_e_t controller_button(int input);
double current_value(driver_values *driver, int param);

void update_parameter();
void update_name();
void update_cursor();
void update_battery();
