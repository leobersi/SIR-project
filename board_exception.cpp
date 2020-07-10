
//#define DOCTEST_CONFIG_DISABLE
// file "board_exception.cpp"
#ifndef BOARD_EXCEPTION_CPP
#define BOARD_EXCEPTION_CPP
#include "board_exception.h"
#include "Common.h"

Board::Board(int n)
    : n_{n}, board_(n, std::vector<State>(n, State::Susceptible)) {
  assert(n > 2);
}

State Board::operator()(int const &i, int const &j) const {
  return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i][j] : State::Removed;
}
State &Board::operator()(int const &i, int const &j) {
  assert(i >= 0 && i < n_ && j >= 0 && j < n_);
  return board_[i][j];
}
inline int Board::size() const { return n_; }

// function user-defined for exceptions
Exception::Exception()
    : std::runtime_error("Math error: Attempted to divide by Zero\n") {}

TEST_CASE("testing operator()") {
  Board board(6);
  board(1, 1) = State::Infected;
  CHECK_NOTHROW(board(1, 1) == State::Infected);
}
#endif