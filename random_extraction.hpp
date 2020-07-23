#ifndef RANDOM_EXTRACTION_HPP
#define RANDOM_EXTRACTION_HPP
#include <random>

std::random_device rd;  // obtain a seed for the random number engine
std::mt19937 gen(rd()); // mt_engine seeded with rd()
//gives a real value between 0 and 1 using mersenne_twister engine
std::uniform_real_distribution<> dis(0.0, 1.0);

//user-defined function for having more control on the extraction range
inline int extraction(int const &lower, int const &upper) {
  std::uniform_int_distribution<> dis(lower, upper);
  return dis(gen);
}
#endif //RANDOM_EXTRACTION_HPP