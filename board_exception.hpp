#ifndef BOARD_EXCEPTION_HPP
#define BOARD_EXCEPTION_HPP
#include <vector>
#include <stdexcept>
#include <cassert>

enum class State : char { Susceptible, Infected, Removed };

class Board {
  int n_;
  std::vector<std::vector<State>> board_;

public:
  Board(int n)
  : n_{n}, board_(n, std::vector<State>(n, State::Susceptible)) {
  assert(n > 2);
  }

  State operator()(int const& i, int const& j) const {
  return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i][j] : State::Removed;
  }

  State &operator()(int const& i, int const& j) {
  assert(i >= 0 && i < n_ && j >= 0 && j < n_);
  return board_[i][j];
  }

  int size() const { return n_; }
};


/////////////////////////////////////////////////////////////////////////////////


// function user-defined for exceptions
class Exception : public std::runtime_error {
public:
  Exception();
};

Exception::Exception()
    : std::runtime_error("Math error: Attempted to divide by Zero\n") {}

#endif //BOARD_EXCEPTION_HPP
