#ifndef PRINT_COUNTER_HPP
#define PRINT_COUNTER_HPP
#include "board_exception.hpp"
#include "epidemy.hpp"

#include <iostream>


/////////////////////////////////////////////////////////////////////////////////

// for every State in the board of the type in function input, increases a
// counter variable by 1
int counter(Board const &board, State state) {
  int number = 0;
  int const N = board.size();
  for (int i = 0; i != N; i++) {
    for (int j = 0; j != N; j++) {
      if (board(i, j) == state) {
        ++number;
      }
    }
  }
  return number;
}

/////////////////////////////////////////////////////////////////////////////////


// function to compute actual board's parameters from the counting of states
// (are given ideas of parameters, not the true values)
inline void parameter_computation(Population &pop, Board const &board) {
  Population prev = pop;
  pop.S = counter(board, State::Susceptible);
  pop.I = counter(board, State::Infected);
  pop.R = counter(board, State::Removed);
  int const number_of_people = pop.S + pop.I + pop.R;
  double const &popS = static_cast<double>(pop.S);
  double const &popI = static_cast<double>(pop.I);
  double const &popR = static_cast<double>(pop.R);
  double const &prevI = static_cast<double>(prev.I);
  double const &prevR = static_cast<double>(prev.R);

  // calculate only if denominator is > 0
  if ((pop.R - prev.R) == 0) {
    throw Exception();
  }
  pop.R0_ = number_of_people * (1 + ((popI - prevI) / (popR - prevR))) / popS;
  pop.Beta =
      (((popR - prevR) + (popI - prevI)) / (popS * popI) * number_of_people);
  pop.gamma_ = ((popR - prevR) / popI);
  // computing medium values for parameters that will be showned
  pop.medium_R0 = (pop.R0_ + prev.R0_) * 0.5;
  pop.medium_gamma = (pop.gamma_ + prev.gamma_) * 0.5;
  pop.medium_Beta = (pop.Beta + prev.Beta) * 0.5;
}

/////////////////////////////////////////////////////////////////////////////////


// print a daily template of statistics of the theorical values of people in
// each state, and of the observed random evolution of the board
inline void print_statistics(int const &day, Population const &p0,
                             Population const &virtual_p0) {
  std::cout << "\033[2J\033[1;1H";
  std::cout << "at day " << day << ", the number of the population is "
            << p0.S + p0.I + p0.R << '\n';
  std::cout << "Expected parameters are: " << '\n';
  std::cout << "R0: " << p0.R0_ << ", Beta: " << p0.Beta
            << ", Gamma: " << p0.gamma_ << '\n';
  std::cout << "Expected S: " << p0.S << '\n';
  std::cout << "Expected I: " << p0.I << '\n';
  std::cout << "Expected R: " << p0.R << '\n';
  std::cout << "Today's prevision % people Involved at the end of epidemy : "
            << R_limit(p0.R0_) << '\n';
  std::cout
      << "Today's prevision % people Supsceptible at the end of epidemy : "
      << 1 - R_limit(p0.R0_) << '\n';
  std::cout << '\n' << '\n';
  std::cout << "Obtained parameters are: " << '\n';
  std::cout << "R0: " << virtual_p0.medium_R0
            << ", Beta: " << virtual_p0.medium_Beta
            << ", Gamma: " << virtual_p0.medium_gamma << '\n';
  std::cout << "Obtained S: " << virtual_p0.S << '\n';
  std::cout << "Obtained I: " << virtual_p0.I << '\n';
  std::cout << "Obtained R: " << virtual_p0.R << '\n';
}

/////////////////////////////////////////////////////////////////////////////////


// not used in this project. Useful only for print in terminal
void print(Board const &board) {
  std::cout << "\033[2J\033[1;1H";
  for (int i = 1; i < (board.size() + 1); ++i) {
    for (int j = 1; j < (board.size() + 1); ++j) {
      if (board(i, j) == State::Susceptible) {
        std::cout << "=";
      }
      if (board(i, j) == State::Infected) {
        std::cout << "||";
      }
      if (board(i, j) == State::Removed) {
        std::cout << "-";
      }
    }
    std::cout << "\n";
  }
}
#endif //PRINT_COUNTER_HPP