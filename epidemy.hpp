#ifndef EPIDEMY_HPP
#define EPIDEMY_HPP
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

struct Population {
  double prob_sus;      // probability of being susceptible
  double prob_inf;      //    ''      ''   ''  infected
  double prob_rem;      //    ''      ''   ''  removed
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

/////////////////////////////////////////////////////////////////////////////////////////////

// class to to describe the epidemic course
class epidemy {
  Population m_p0;
  int m_number_of_people;

public:

  //////////////////////////////////////////////////////

  epidemy(Population p0, int const &number_of_people);
  
  /////////////////////////////////////////////////////
  
  Population updating(Population const &prev) const;
  
  /////////////////////////////////////////////////////
};

//definitions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline epidemy::epidemy(Population p0, int const &number_of_people)
    : m_p0{p0}, m_number_of_people{number_of_people} {};


/////////////////////////////////////////////////////////////////////////////////


// this method reads a population in input and computes a day-by-day update
// of probabilities, parameters and expected population, returning a population
inline Population epidemy::updating(Population const &prev) const {
  Population p;
  // probabilities based on SIR equations
  p.prob_sus = prev.prob_sus - prev.R0_ * prev.gamma_ * prev.prob_sus * prev.prob_inf;
  p.prob_inf = prev.prob_inf + (prev.R0_ * prev.prob_sus - 1) * prev.gamma_ * prev.prob_inf;
  p.prob_rem = prev.prob_rem + prev.gamma_ * prev.prob_inf;
  // s, i and r are normalized, so their sum should be 1
  double sum = std::round(p.prob_inf + p.prob_sus + p.prob_rem);
  assert(sum == 1);
  // parameters computed with formulas for finite differences
  p.R0_ = (1 + (p.prob_inf - prev.prob_inf) / (p.prob_rem - prev.prob_rem)) / p.prob_sus;
  p.Beta = ((p.prob_rem - prev.prob_rem) + (p.prob_inf - prev.prob_inf)) / (p.prob_sus * p.prob_inf);
  p.gamma_ = ((p.prob_rem - prev.prob_rem) / p.prob_inf);
  // number of people based on probabilities of being of that State
  p.I = std::round(p.prob_inf * m_number_of_people);
  p.R = std::round(p.prob_rem * m_number_of_people);
  p.S = m_number_of_people - p.I - p.R;
  return p;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/////////////////////////////////////////////////////////////////////////////////////////////

// function to calculate % people involved at the end of epidemy
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

#endif //EPIDEMY_HPP
