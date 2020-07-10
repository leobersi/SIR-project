//#define DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#ifndef EVOLUTION_H
#define EVOLUTION_H
#include "board_exception.h"
#include "epidemy.h"
#include "random_estraction.h"


void Initialize(Board &board, double const probability) {
  for (int i = 0; i != board.size(); ++i) {
    for (int j = 0; j != board.size(); ++j) {
      if (dis(gen) < probability) {
        board(i, j) = State::I;
      }
    }
  }
}
// test this function

void evolve(Board &board, Population const &population) {
  double const probability_infection = population.i;
  double const probability_recovered = population.r;
  for (int r = 1; r != board.size() - 1; ++r) {
    for (int c = 1; c != board.size() - 1; ++c) {
      if (board(r, c) == State::I) {
        for (int i = r - 1; i != r + 1; ++i) {
          for (int j = c - 1; j != c + 1; ++j) {
            if (board(i, j) == State::S) {
              if (dis(gen) < probability_infection) {
                board(i, j) = State::I;
              }
            }
          }
        }
        if (dis(gen) < probability_recovered) {
          board(r, c) = State::R;
        }
      }
    }
  }
}
// test this function

void move(Board &board) {
  for (int r = 1; r != board.size() - 1; ++r) {
    for (int c = 1; c != board.size() - 1; ++c) {
      std::swap(board(r, c), board(estraction(r - 1, r + 1),
                                   estraction(c - 1, c + 1))); // 1 = range
    }
  }
}

TEST_CASE("testing Initialize") {
  Board board(6);
  auto board_ = board;
  double probability = 0;
  CHECK_NOTHROW(Initialize(board, probability));
  Initialize(board, probability);
  CHECK(board(1,2) == board_(1,2)); //board == board_
  CHECK(board(2,1) == board_(2,1));
  CHECK(board(3,4) == board_(3,4));
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
  CHECK(board(1,2) == board_(1,2)); //board == board_
  CHECK(board(2,1) == board_(2,1));
  CHECK(board(3,4) == board_(3,4));
}
//banal tests for move
TEST_CASE("testing move") {
  Board board(6);
  auto board_ = board;
  double probability = 0;
  Initialize(board, probability);
  CHECK_NOTHROW(move(board));
  CHECK(board(1,2) == board_(1,2)); //board == board_
  CHECK(board(2,1) == board_(2,1));
  CHECK(board(3,4) == board_(3,4));
}
#endif