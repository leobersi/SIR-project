//#define DOCTEST_CONFIG_DISABLE
// file "epidemy.h"
#ifndef EPIDEMY_H
#define EPIDEMY_H

#include "Common.h"

struct Population {
  double s;      // probability of being susceptible
  double i;      //    ''      ''   ''  infected
  double r;      //    ''      ''   ''  removed
  int S;         // people susceptible
  int I;         // people infected
  int R;         // people removed
  double R0_;    // daily estimation value of Net Reproduction Rate
  double Beta;   // daily estimation value of Actual Contact Rate
  double gamma_; // daily estimation value of Effective Removal Rate
  double
      medium_R0; // value of Net Reproduction Rate in a temporal step of 2 days
  double
      medium_Beta; // value of Actual Contact Rate in a temporal step of 2 days
  double medium_gamma; // value of Effective Removal Rate in a temporal step of
                       // 2 days
};

// class to to describe the epidemic course
class epidemy {
  Population m_p0;
  int m_number_of_people;

public:
  epidemy(Population p0, int const &number_of_people);
  auto updating(Population const &prev) const;
};

// function to calculate % people involved in epidemy
// computing an approssimate solution for a trascendent equation :
// r_inf = 1-e^(-R0 * r_inf)
auto R_limit(double const &R0) {
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


TEST_CASE("testing R_limit") {
  // testing some outputs
  double R0 = 4.64042;
  CHECK_NOTHROW(R_limit(R0));
  auto A = R_limit(R0);
  CHECK(A == doctest::Approx(0.9898)); // no ".epsilon()" because is
  R0 = 4.61337;                        // a vector of 10000 elements,
  CHECK_NOTHROW(R_limit(R0));          // so only 4 decimals allowed
  auto B = R_limit(R0);
  CHECK(B == doctest::Approx(0.9895));
  R0 = std::numeric_limits<double>::infinity(); // Returns the special value
  CHECK_NOTHROW(R_limit(R0) == 1); //"positive infinity", as represented
} // by the floating-point type T

#endif
