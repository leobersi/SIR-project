//#define DOCTEST_CONFIG_DISABLE
#ifndef EVOLUTION_H
#define EVOLUTION_H
#include "board_exception.h"
#include "epidemy.h"
#include "random_estraction.h"
#include "Common.h"

// a board of S is initialized with a random number of I, based on probabilities
void Initialize(Board &board, double const probability) {
  for (int i = 0; i != board.size(); ++i) {
    for (int j = 0; j != board.size(); ++j) {
      if (dis(gen) < probability) {
        board(i, j) = State::Infected;
      }
    }
  }
}
// every day the probability of become infect or removed is computed by updating
// methods and is applied thanks to the function eolve.
void evolve(Board &board, Population const &population) {
  double const probability_infection = population.i;
  double const probability_recovered = population.r;
  // for every infected person in the board, based on the probabilities to get
  // removed on that day, a random estraction is done to decide if it will
  // become removed
  for (int r = 0; r != board.size(); ++r) {
    for (int c = 0; c != board.size(); ++c) {
      if (board(r, c) == State::Infected) {
        // if the person is in the board, it cannot infect. Can only become
        // removed
        if (r == 0 || c == 0 || r == board.size() - 1 ||
            c == board.size() - 1) {
          if (dis(gen) < probability_recovered) {
            board(r, c) = State::Removed;
          }
        } else {
          // otherwise a counting starts to decide the number of infected from
          // the people around, based on probabilities to get infected in that
          // day
          for (int i = r - 1; i != r + 1; ++i) {
            for (int j = c - 1; j != c + 1; ++j) {
              if (board(i, j) == State::Susceptible) {
                if (dis(gen) < probability_infection) {
                  board(i, j) = State::Infected;
                }
              }
            }
          }
          if (dis(gen) < probability_recovered) {
            board(r, c) = State::Removed;
          }
        }
      }
    }
  }
}
// function to swap status in board with a range of 1, simulating movement of
// people
void move(Board &board) {
  for (int r = 1; r != board.size() - 1; ++r) {
    for (int c = 1; c != board.size() - 1; ++c) {
      std::swap(board(r, c), board(extraction(r - 1, r + 1),
                                   extraction(c - 1, c + 1))); // range = 1
    }
  }
}

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
  pop.i = 0;
  pop.r = 0;
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
#endif