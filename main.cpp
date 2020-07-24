#include "evolution.hpp"
#include "print_counter.hpp"
#include <SFML/Graphics.hpp>

//**********************************************************
// include only for simple execution by terminal version ***
//                                                        **
//#include <chrono>                                   
//#include <thread>
/////*******************************************************

int main() {

  /*
  // only for simple execution by terminal
  //
  const int total_days = 400;
  const int board_side = 200;
  const int number_of_people = board_side * board_side;
  Board board(board_side);
  Population p0{};
  p0.prob_inf = 0.0045;
  p0.prob_sus = 0.9955;
  p0.Beta = 0.151;
  p0.gamma_ = 0.0351;
  p0.R0_ = 4.58;
  Population virtual_p0{};
  epidemy covid(p0, number_of_people);

  for (int day = 1; day <= total_days; day++) {
    p0 = covid.updating(p0);
    if (day == 1) {
      Initialize(board, p0.prob_inf);
      print(board);
      virtual_p0.S = counter(board, State::Susceptible);
      virtual_p0.I = counter(board, State::Infected);
      virtual_p0.R = counter(board, State::Removed);
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

*/
//main with terminale implemented with SFML
//
 const int dim = 300;
 const int number_of_people = dim * dim;
 Board board(dim);
 Population p0{};
 p0.prob_inf = 0.0045;
 p0.prob_sus = 0.9955;
 p0.Beta = 0.151;
 p0.gamma_ = 0.0351;
 p0.R0_ = 4.58;
 Population virtual_p0{};
 epidemy covid(p0, number_of_people);

 Initialize(board, p0.prob_inf);
 virtual_p0.S = counter(board, State::Susceptible);
 virtual_p0.I = counter(board, State::Infected);
 virtual_p0.R = counter(board, State::Removed);
 sf::RenderWindow window(sf::VideoMode(dim * 4 - 80, dim + 6), "Simulation of an epidemic");
 window.clear(sf::Color::White);
 sf::RectangleShape rectangle_s(sf::Vector2f(1.f, 1.f));
 rectangle_s.setFillColor(sf::Color::Green);
 sf::RectangleShape rectangle_i(sf::Vector2f(1.f, 1.f));
 rectangle_i.setFillColor(sf::Color::Red);
 sf::RectangleShape rectangle_r(sf::Vector2f(1.f, 1.f));
 rectangle_r.setFillColor(sf::Color::Blue);
 sf::RectangleShape rectangle_s_leg(sf::Vector2f(4.f, 4.f));
 rectangle_s_leg.setFillColor(sf::Color::Green);
 sf::RectangleShape rectangle_i_leg(sf::Vector2f(4.f, 4.f));
 rectangle_i_leg.setFillColor(sf::Color::Red);
 sf::RectangleShape rectangle_r_leg(sf::Vector2f(4.f, 4.f));
 rectangle_r_leg.setFillColor(sf::Color::Blue);
 sf::CircleShape graph_s(1.f);
 graph_s.setFillColor(sf::Color::Green);
 sf::CircleShape graph_i(1.f);
 graph_i.setFillColor(sf::Color::Red);
 sf::CircleShape graph_r(1.f);
 graph_r.setFillColor(sf::Color::Blue);
 sf::Font font;
 if (!font.loadFromFile("arial.ttf")) {
   std::cout << "Error loading file\n";
   system(" pause ");
 }
 sf::Text text1;
 text1.setFont(font);
 text1.setString("S u s c e p t i b l e");
 text1.setCharacterSize(12);
 text1.setFillColor(sf::Color::Black);
 text1.setPosition(dim * 3 + 26, 0);
 sf::Text text2;
 text2.setFont(font);
 text2.setString("I n f e c t e d");
 text2.setCharacterSize(12);
 text2.setFillColor(sf::Color::Black);
 text2.setPosition(dim * 3 + 26, 12);
 sf::Text text3;
 text3.setFont(font);
 text3.setString("R e c o v e r e d");
 text3.setCharacterSize(12);
 text3.setFillColor(sf::Color::Black);
 text3.setPosition(dim * 3 + 26, 24);

 int day = 0;
 while (window.isOpen()) {
   sf::Event evnt;
   while (window.pollEvent(evnt)) {
     if (evnt.type == sf::Event::Closed)
       window.close();
   }
   graph_s.setPosition(dim + day + 6, dim - (virtual_p0.S / (dim ^ 2)));
   window.draw(graph_s);
   graph_i.setPosition(dim + day + 6, dim - (virtual_p0.I / (dim ^ 2)));
   window.draw(graph_i);
   graph_r.setPosition(dim + day + 6, dim - (virtual_p0.R / (dim ^ 2)));
   window.draw(graph_r);
   rectangle_s_leg.setPosition(dim * 3 + 20, 6);
   window.draw(rectangle_s_leg);
   rectangle_i_leg.setPosition(dim * 3 + 20, 18);
   window.draw(rectangle_i_leg);
   rectangle_r_leg.setPosition(dim * 3 + 20, 30);
   window.draw(rectangle_r_leg);
   window.draw(text1);
   window.draw(text2);
   window.draw(text3);
   for (int i = 0; i != board.size(); ++i) {
     for (int j = 0; j != board.size(); ++j) {
       if (board(i, j) == State::Susceptible) {
         rectangle_s.setPosition(j + 3, i + 3);
         window.draw(rectangle_s);
       }
       if (board(i, j) == State::Infected) {
         rectangle_i.setPosition(j + 3, i + 3);
         window.draw(rectangle_i);
       }
       if (board(i, j) == State::Removed) {
         rectangle_r.setPosition(j + 3, i + 3);
         window.draw(rectangle_r);
       }
     }
   }
   window.display();
   p0 = covid.updating(p0);
   evolve(board, p0);
   move(board);
   try {
     parameter_computation(virtual_p0, board);
   } catch (Exception &e) {
     std::cout << "Exception occurred" << '\n' << e.what();
   }
   print_statistics(day, p0, virtual_p0);
   assert(virtual_p0.S + virtual_p0.I + virtual_p0.R == number_of_people);
   if (virtual_p0.I == 0) {

     graph_s.setPosition(dim + day + 6, dim - (virtual_p0.S / (dim ^ 2)));
     window.draw(graph_s);
     graph_i.setPosition(dim + day + 6, dim - (virtual_p0.I / (dim ^ 2)));
     window.draw(graph_i);
     graph_r.setPosition(dim + day + 6, dim - (virtual_p0.R / (dim ^ 2)));
     window.draw(graph_r);
     rectangle_s_leg.setPosition(dim * 3 + 20, 6);
     window.draw(rectangle_s_leg);
     rectangle_i_leg.setPosition(dim * 3 + 20, 18);
     window.draw(rectangle_i_leg);
     rectangle_r_leg.setPosition(dim * 3 + 20, 30);
     window.draw(rectangle_r_leg);
     window.draw(text1);
     window.draw(text2);
     window.draw(text3);
     for (int i = 0; i != board.size(); ++i) {
       for (int j = 0; j != board.size(); ++j) {
         if (board(i, j) == State::Susceptible) {
           rectangle_s.setPosition(j + 3, i + 3);
           window.draw(rectangle_s);
         }
         if (board(i, j) == State::Infected) {
           rectangle_i.setPosition(j + 3, i + 3);
           window.draw(rectangle_i);
         }
         else {
           rectangle_r.setPosition(j + 3, i + 3);
           window.draw(rectangle_r);
       }
       }
     }
     window.display();
     try {
       parameter_computation(virtual_p0, board);
     } catch (Exception &e) {
       std::cout << "Exception occurred" << '\n' << e.what();
     }
     print_statistics(day, p0, virtual_p0);
     assert(virtual_p0.S + virtual_p0.I + virtual_p0.R == number_of_people);
     window.close();
   }
   day = day + 1;
 }
}