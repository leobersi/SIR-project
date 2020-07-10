//#define DOCTEST_CONFIG_DISABLE
//#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#ifndef EPIDEMY_CPP
#define EPIDEMY_CPP
#include "epidemy.h"

#include <cassert>
#include <cmath>
#include <iostream>

epidemy::epidemy(Population p0, int const &number_of_people)
    : m_p0{p0}, m_number_of_people{number_of_people} {};

inline auto epidemy::evolve(Population const &prev) const {
  Population p;

  p.s = prev.s - prev.R0_ * prev.gamma_ * prev.s * prev.i;
  p.i = prev.i + (prev.R0_ * prev.s - 1) * prev.gamma_ * prev.i;
  p.r = prev.r + prev.gamma_ * prev.i;
  double sum = std::round(p.i + p.s + p.r);
  assert(sum == 1);
  p.R0_ = (1 + (p.i - prev.i) / (p.r - prev.r)) / p.s;
  p.Beta = ((p.r - prev.r) + (p.i - prev.i)) / (p.s * p.i);
  p.gamma_ = ((p.r - prev.r) / p.i);

  p.I = std::round(p.i * m_number_of_people);
  p.R = std::round(p.r * m_number_of_people);
  p.S = m_number_of_people - p.I - p.R;
  return p;
}

TEST_CASE("testing evolve") {
  Population p{};
  p.i = 0.01;
  p.s = 0.99;
  p.Beta = 0.151;
  p.gamma_ = 0.0351;
  p.R0_ = 4.58;
  epidemy epidemy(p, 400);
  int test_loop = 20;
  for (int i = 0; i != test_loop; i++) {
    CHECK_NOTHROW(epidemy.evolve(p));
    p = epidemy.evolve(p);
    CHECK_NOTHROW(p.s > 0);
    CHECK_NOTHROW(p.i > 0);
    CHECK_NOTHROW(p.r > 0);
    double R0 = 400*p.Beta/p.gamma_;
    CHECK_NOTHROW(p.R0_ == doctest::Approx(R0).epsilon(0.001));               
    CHECK_NOTHROW(p.R0_ > 1);           // real R0 must be > 1 for epidemy 
    CHECK_NOTHROW(p.medium_Beta == 0); //those values are not touched by evolve()
    CHECK_NOTHROW(p.medium_gamma == 0);
    CHECK_NOTHROW(p.medium_R0 == 0);
  }
}
#endif