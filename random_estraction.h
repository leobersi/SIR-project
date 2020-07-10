//#define DOCTEST_CONFIG_DISABLE
// file "random_estraction.h"
#ifndef RANDOM_ESTRACTION_H
#define RANDOM_ESTRACTION_H
#include <random>

std::random_device rd;  // obtain a seed for the random number engine
std::mt19937 gen(rd()); // mt_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0, 1.0);

inline int estraction(int const &lower, int const &upper) {
  std::uniform_int_distribution<> dis(lower, upper);
  return dis(gen);
}
#endif