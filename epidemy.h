//#define DOCTEST_CONFIG_DISABLE
// file "epidemy.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#ifndef EPIDEMY_H
#define EPIDEMY_H


#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Population {
  double s;
  double i;
  double r;
  int S;
  int I;
  int R;
  double R0_;
  double Beta;
  double gamma_;
  double medium_R0;
  double medium_Beta;
  double medium_gamma;
};

class epidemy {
  Population m_p0;
  int m_number_of_people;

public:
  epidemy(Population p0, // constructor declaration
          int const &number_of_people);
  auto
  evolve(Population const &prev) const; // member function evolve declaration
};

// function to calculate % people involved in epidemy
// computing an approssimate solution for a trascendent equation
auto r_limit(double const &R0) {
  std::vector<double> rtest(10000);
  std::generate(rtest.begin(), rtest.end(), [i = 0.]() mutable {
    return i += 0.0001;
  });                   // filling the vector
  for (auto &x : rtest) // computing the deficit
  {
    x = std::abs(x - 1 + std::exp(-R0 * x));
  }
  auto first = rtest.begin();
  auto minimum_it = min_element(rtest.begin(), rtest.end());
  double const r_min_value =
      (std::distance(first, minimum_it) / 10000.); // approssimation of solution
  return r_min_value;
}



// testing
TEST_CASE("testing r_limit") {
  // testing some outputs
  double R0 = 4.64042;
  CHECK_NOTHROW(r_limit(R0));
  auto A = r_limit(R0);
  CHECK(A == doctest::Approx(0.9898)); // no ".epsilon()" because is
  R0 = 4.61337;                        // a vector of 10000 elements,
  CHECK_NOTHROW(r_limit(R0));          // so only 4 decimals allowed
  auto B = r_limit(R0);
  CHECK(B == doctest::Approx(0.9895));
  R0 = std::numeric_limits<double>::infinity(); // Returns the special value
  CHECK_NOTHROW(r_limit(R0) == 1);              //"positive infinity", as represented
}                                               // by the floating-point type T

#endif