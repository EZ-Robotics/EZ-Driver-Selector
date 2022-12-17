#include "main.h"

inline std::string unselected_cursor = "#";
inline std::string selected_cursor = "&";
inline std::string cursor = unselected_cursor;

inline int cursor_placement = 1;

double add_driver_default(driver_values *driver, int param, double added);
double current_value(driver_values *driver, int param);

void update_parameter();
void update_name();
void update_cursor();
void update_battery();
