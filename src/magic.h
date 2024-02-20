// This header serves only for magic numbers generation so
// it does not need to be included with everything else

#ifndef MAGIC_H
#define MAGIC_H

#include "general.h"
#include "attacks.h"
#include <stdint.h> // Needed for uint32_t type

// For reproducibilty purposes we coded our own 32 bit RNG using
// X-OR shift algorithm
uint32_t generate_random_number(int, int, int);

U64 generate_magic_candidate();
U64 generate_magic_number();

#endif