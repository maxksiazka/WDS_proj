#ifndef DIFF_PRESSURE_H_
#define DIFF_PRESSURE_H_
#include <stdbool.h>
extern volatile float latest_pressure_pa;
extern volatile bool new_data_available;

void diff_pressure_init();
#endif // DIFF_PRESSURE_H_
