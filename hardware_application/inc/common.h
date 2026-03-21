#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>

#ifndef NDEBUG
#define print_debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define print_debug(...) // no-op
#endif



#endif  // COMMON_H_
