#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>

#ifndef NDEBUG
#define print_debug(...) fprintf(stderr, __VA_ARGS__)
#endif
#define LED_PIN CYW43_WL_GPIO_LED_PIN



#endif  // COMMON_H_
