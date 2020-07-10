///#define DOCTEST_CONFIG_DISABLE
// file "board_exception.cpp"
#ifndef BOARD_EXCEPTION_CPP
#define BOARD_EXCEPTION_CPP
#include "board_exception.h"
#include <cassert>
#include <iostream>
#include <vector>

Board::Board(int n) : n_{n}, board_(n, std::vector<State>(n, State::S)) {
  assert(n > 2);
}

State Board::operator()(int i, int j) const {
  return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i][j] : State::R;
}
State &Board::operator()(int i, int j) {
  assert(i >= 0 && i < n_ && j >= 0 && j < n_);
  return board_[i][j];
}
int Board::size() const { return n_; }

Exception::Exception()
    : std::runtime_error("Math error: Attempted to divide by Zero\n") {}

#endif