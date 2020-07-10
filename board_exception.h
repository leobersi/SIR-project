//#define DOCTEST_CONFIG_DISABLE
// file "Board_Exception.h"
#ifndef BOARD_EXCEPTION_H
#define BOARD_EXCEPTION_H
#include "Common.h"
#include <vector>

enum class State : char { S, I, R };

class Board {
  int n_;
  std::vector<std::vector<State>> board_;

public:
  Board(int n);

  State operator()(int i, int j) const;

  State &operator()(int i, int j);

  int size() const;
};

class Exception : public std::runtime_error {
public:
  Exception();
};
#endif