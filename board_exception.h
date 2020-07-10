
//#define DOCTEST_CONFIG_DISABLE
// file "Board_Exception.h"
#ifndef BOARD_EXCEPTION_H
#define BOARD_EXCEPTION_H
#include "Common.h"


enum class State : char { Susceptible, Infected, Removed };

class Board {
  int n_;
  std::vector<std::vector<State>> board_;

public:
  Board(int n);

  State operator()(int const& i, int const& j) const;

  State &operator()(int const& i, int const& j);

  inline int size() const;
};

class Exception : public std::runtime_error {
public:
  Exception();
};
#endif
