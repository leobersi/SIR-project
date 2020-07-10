//#define DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT
#include "Common.h"
#include "board_exception.cpp"
#include "board_exception.h"
#include "doctest.h"
#include "epidemy.cpp"
#include "epidemy.h"
#include "evolution.h"
#include "print_counter.h"
#include "random_estraction.h"

int main(int argc, char **argv) {

  doctest::Context context;
  context.setOption("abort-after", 4);
  context.applyCommandLine(argc, argv);
  context.setOption("no-breaks", true);
  context.setAsDefaultForAssertsOutOfTestCases();
  int res = context.run();              // run test cases unless with --no-run
  if (context.shouldExit())
    return res;

  return res;

  const int total_days = 60;
  const int board_side = 200;
  const int number_of_people = board_side * board_side;
  Board board(board_side);
  Population p0{};
  p0.i = 0.0045;
  p0.s = 0.9955;
  p0.Beta = 0.151;
  p0.gamma_ = 0.0351;
  p0.R0_ = 4.58;
  Population virtual_p0{};
  epidemy covid(p0, number_of_people);

  for (int day = 1; day <= total_days; day++) {
    p0 = covid.evolve(p0);
    if (day == 1) {
      Initialize(board, p0.i);
      print(board);
      virtual_p0.S = counter(board, State::S);
      virtual_p0.I = counter(board, State::I);
      virtual_p0.R = counter(board, State::R);
    }

    evolve(board, p0);
    move(board);
    print(board);
    try {
      parameter_computation(virtual_p0, board);
    } catch (Exception &e) {
      std::cout << "Exception occurred" << '\n' << e.what();
    }
    print_statistics(day, p0, virtual_p0);
    assert(virtual_p0.S + virtual_p0.I + virtual_p0.R == number_of_people);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}