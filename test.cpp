#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "evolution.hpp"
#include "print_counter.hpp"

// .ccp file used for tests with doctest

/////////////////////////////////////////////////////////////////////////////////
//from "epidemy.hpp"

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


/////////////////////////////////////////////////////////////////////////////////
//from "print_counter.hpp"

TEST_CASE("testing parameter_computation()") {
  Board board(6);
  Population pop{};
  Board board2(6);
  pop.R = 1;
  board2(1, 2) = State::Infected;
  board2(2, 2) = State::Removed;
  board2(4, 2) = State::Removed;
  parameter_computation(pop, board);
  CHECK_NOTHROW(pop.R == 1);
}
/////////////////////////////////////////////////////////////////////////////////
//from "epidemy.hpp"

TEST_CASE("testing updating()") {
  Population p{};
  p.prob_inf = 0.01;
  p.prob_sus = 0.99;
  p.Beta = 0.151;
  p.gamma_ = 0.0351;
  p.R0_ = 4.58;
  epidemy epidemy(p, 400);
  int test_loop = 20;
  for (int i = 0; i != test_loop; i++) {
    CHECK_NOTHROW(epidemy.updating(p));
    p = epidemy.updating(p);
    CHECK_NOTHROW(p.prob_sus > 0);
    CHECK_NOTHROW(p.prob_inf > 0);
    CHECK_NOTHROW(p.prob_rem > 0);
    double R0 = 400 * p.Beta / p.gamma_;
    CHECK_NOTHROW(p.R0_ == doctest::Approx(R0).epsilon(0.001));
    CHECK_NOTHROW(p.R0_ > 1); // real R0 must be > 1 for epidemy
    CHECK_NOTHROW(p.medium_Beta ==
                  0); // those values are not touched by evolve()
    CHECK_NOTHROW(p.medium_gamma == 0);
    CHECK_NOTHROW(p.medium_R0 == 0);
  }
}

/////////////////////////////////////////////////////////////////////////////////
//from "board_exception.hpp"

TEST_CASE("testing operator()") {
  Board board(6);
  board(1, 1) = State::Infected;
  CHECK_NOTHROW(board(1, 1) == State::Infected);
}

/////////////////////////////////////////////////////////////////////////////////
//from "evolution.hpp"

TEST_CASE("testing Initialize") {
  Board board(6);
  auto board_ = board;
  double probability = 0;
  CHECK_NOTHROW(Initialize(board, probability));
  Initialize(board, probability);
  CHECK(board(1, 2) == board_(1, 2)); // board == board_
  CHECK(board(2, 1) == board_(2, 1));
  CHECK(board(3, 4) == board_(3, 4));
}
TEST_CASE("testing Evolve") {
  Board board(6);
  auto board_ = board;
  double probability = 0;
  Initialize(board, probability);
  Population pop{};
  pop.prob_inf = 0;
  pop.prob_rem = 0;
  CHECK_NOTHROW(evolve(board, pop));
  evolve(board, pop);
  CHECK(board(1, 2) == board_(1, 2)); // board == board_
  CHECK(board(2, 1) == board_(2, 1));
  CHECK(board(3, 4) == board_(3, 4));
}
// banal tests for move
TEST_CASE("testing move") {
  Board board(6);
  auto board_ = board;
  double probability = 0;
  Initialize(board, probability);
  CHECK_NOTHROW(move(board));
  CHECK(board(1, 2) == board_(1, 2)); // board == board_
  CHECK(board(2, 1) == board_(2, 1));
  CHECK(board(3, 4) == board_(3, 4));
}