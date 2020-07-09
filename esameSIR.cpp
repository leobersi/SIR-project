//#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <random>
#include <stdexcept>


using namespace std::chrono_literals;

enum class State : char { S, I, R };

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0, 1.0);

struct Population{
 double s;
 double i;
 double r;
 int S;
 int I;
 int R;
 double R0_;
 double Beta;
 double gamma_;
 double medium_R0;
 double medium_Beta;
 double medium_gamma;
};

class epidemy {
 Population m_p0;
 int m_number_of_people;

public: 
 epidemy(Population p0, int const& number_of_people) : m_p0{p0}, m_number_of_people{number_of_people} {};

auto evolve(Population const& prev) const 
 { 
   Population p;
      
   p.s = prev.s - prev.R0_ * prev.gamma_ * prev.s * prev.i ;
   p.i = prev.i + (prev.R0_ * prev.s - 1) * prev.gamma_ * prev.i ;
   p.r = prev.r + prev.gamma_ * prev.i ;

   double sum = std::round(p.i + p.s + p.r);
   assert(sum == 1); 

   p.R0_ = ( 1 + (p.i - prev.i) / (p.r - prev.r) )  /  p.s;
   p.Beta = ( (p.r - prev.r) + (p.i - prev.i) ) / ( p.s  * p.i );
   p.gamma_ = ( (p.r - prev.r) / p.i );
      

   p.I = std::round(p.i * m_number_of_people);
   p.R = std::round(p.r * m_number_of_people);
   p.S = m_number_of_people - p.I - p.R;
   return p;
 }
};

/*void printPopulation (Population const& pop, int const& day) {
 std::cout << "Day  ||    S   ||    I    ||    R\n";

  std::cout<< "giorno " << day + 1 << "  " << pop.S << " , " << pop.I << " , " <<pop.R 
  << " "<< pop.s << " , " << pop.i << " , " <<pop.r<< '\n';
  }*/


auto r_limit (double const& R0) { //inserire parametri variabili
  std::vector<double> rtest (10000);
std::generate(rtest.begin(), rtest.end(),[i=0.]()mutable //filling rtest
  {
   return i += 0.0001;
  } ); 
for (auto& x : rtest) //computing the deficit
  {
    x = std::abs( x - 1 + std::exp(-R0 * x) );
  }
  auto first = rtest.begin();
  auto minimum_it = min_element (rtest.begin(),rtest.end());
  double const r_min_value = ( std::distance(first, minimum_it) / 10000.);
  return r_min_value;
}

class Board {
    int n_;
    std::vector<std::vector<State>> board_;
public:
    Board(int n) : n_{ n }, board_(n, std::vector<State>(n, State::S))
    {assert(n > 2); }

    State operator() (int i, int j) const {
        return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i][j]
            : State::R;

    }
    State& operator() (int i, int j) {
        assert(i >= 0 && i < n_ && j >= 0 && j < n_);
        return board_[i][j];
    }
    int size() const {
        return n_;
    }
};

void print(Board const& board) {
    std::cout << "\033[2J\033[1;1H";
    for (int i = 1; i < (board.size()+1); ++i) {
        for (int j = 1; j < (board.size()+1); ++j) {
            if(board(i,j) == State::S) {
                std::cout<<"=";
            }
            if(board(i,j) == State::I) {
                std::cout<<"||";
            }
            if(board(i,j) == State::R) {
                std::cout<<"-";
            }
         } std::cout << "\n";  
        } 
    }

void Initialize(Board& board, double const probability) {
    for (int i = 0; i != board.size(); ++i) {
    for (int j = 0; j != board.size(); ++j) {
        if (dis(gen) < probability) {
            board(i, j) = State::I;
            }
        }
    }

}


void evolve(Board& board, Population const& population)
{
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

int estraction(int const& lower, int const& upper) {
std::uniform_int_distribution<> dis(lower, upper);
return dis(gen);
}

void move(Board& board) {
    for (int r = 1; r != board.size() - 1; ++r) {
    for (int c = 1; c != board.size() - 1; ++c) {
      std::swap(board(r, c), board(estraction(r - 1, r + 1), estraction(c - 1, c + 1)));//1 = range
     }
   }
}


int counter(Board const& board, State state) {
  int number = 0;
  int const N = board.size();
  for (int i=0; i!=N; i++) {
      for(int j=0; j !=N; j++){
      if (board(i,j) == state) {
      ++number;
     }
    }
  }
  return number;
}

class Exception : public std::runtime_error { 
public: 
    Exception() 
        : std::runtime_error("Math error: Attempted to divide by Zero\n") 
    { 
    } 
}; 

void parameter_computation(Population& pop, Board const& board){
  Population prev = pop;
  pop.S = counter(board, State::S);
  pop.I = counter(board, State::I);
  pop.R = counter(board, State::R);
  int const number_of_people = pop.S + pop.I + pop.R;
  double const& popS = static_cast<double>(pop.S);
  double const& popI = static_cast<double>(pop.I);
  double const& popR = static_cast<double>(pop.R);
  double const& prevI = static_cast<double>(prev.I);
  double const& prevR = static_cast<double>(prev.R);

  //calculate only if denominators are > 0
  if ((pop.R - prev.R) == 0) { 
        throw Exception(); 
    } 
  pop.R0_ = number_of_people * ( 1 + ( (popI - prevI) / (popR - prevR) ) )  /  popS ;
  pop.Beta = ( ( (popR - prevR) + (popI - prevI) ) / ( popS  * popI ) * number_of_people );
  pop.gamma_ = ( (popR - prevR) / popI );
  pop.medium_R0 = ( pop.R0_ + prev.R0_ ) * 0.5;
  pop.medium_gamma = ( pop.gamma_ + prev.gamma_ ) * 0.5;
  pop.medium_Beta = ( pop.Beta + prev.Beta ) * 0.5;
}

void print_statistics(int const& day, Population const& p0, Population const& virtual_p0){
std::cout << "at day " << day << ", the number of the population is " << p0.S+p0.I+p0.R << '\n';
std::cout << "Expected parameters are: " <<'\n';
std::cout << "R0: " << p0.R0_ << ", Beta: " << p0.Beta << ", Gamma: " << p0.gamma_ << '\n';  
std::cout << "Expected S: " << p0.S <<'\n';
std::cout << "Expected I: " << p0.I <<'\n';
std::cout << "Expected R: " << p0.R <<'\n';    
std::cout << "Today's prevision % people Involved at the end of epidemy : " << r_limit(p0.R0_)<<'\n';
std::cout << "Today's prevision % people Supsceptible at the end of epidemy : " << 1 - r_limit(p0.R0_) <<'\n';
std::cout << '\n'<<'\n';
std::cout << "Obtained parameters are: " <<'\n';
std::cout << "R0: " << virtual_p0.medium_R0 << ", Beta: " << virtual_p0.medium_Beta << ", Gamma: " << virtual_p0.medium_gamma << '\n';
std::cout << "Obtained S: " << virtual_p0.S <<'\n';
std::cout << "Obtained I: " << virtual_p0.I <<'\n';
std::cout << "Obtained R: " << virtual_p0.R <<'\n';
}



//main da terminal
int main() {
const int total_days = 60;
const int board_side = 200;
const int number_of_people = board_side * board_side;
Board board(board_side);
Population p0{};
p0.i = 0.001;
p0.s = 0.999;
p0.Beta = 0.151;
p0.gamma_ = 0.0351;
p0.R0_ = 4.58;
Population virtual_p0{};
epidemy covid(p0,number_of_people);

for (int day=1; day<=total_days; day++) {

 p0 = covid.evolve(p0);
 if(day==1){
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
   } catch (Exception& e) {
        std::cout << "Exception occurred" << '\n' 
             << e.what(); 
   } 
 print_statistics(day, p0, virtual_p0);
 assert (virtual_p0.S + virtual_p0.I + virtual_p0.R == number_of_people );
 
 std::this_thread::sleep_for (std::chrono::milliseconds(500));
 }
}
/*

//main grafico
int main() {
    int dim = 200;
    Board board(dim);
    const int number_of_people = board.size()*board.size();
    Population p0{};
    p0.i = 0.001;
    p0.s = 0.999;
    p0.expected_i = 0.001;
    p0.expected_s = 0.999;
    p0.Beta = Beta;
    p0.gamma_ = gamma_;
    p0.R0_ = R0;
    epidemy covid(p0, number_of_people);
    std::cout << "% of people involved in epidemy is: " << std::round(r_limit()) << '\n';
    std::cout << "% of people not involved in epidemy is: " << 1 - std::round(r_limit()) << '\n';
    Initialize(board, p0.i);
    sf::RenderWindow window(sf::VideoMode(dim*3, dim), "Simulation of an epidemic");
    sf::RectangleShape rectangle_s(sf::Vector2f(1.f, 1.f));
    rectangle_s.setFillColor(sf::Color::Green);
    sf::RectangleShape rectangle_i(sf::Vector2f(1.f, 1.f));
    rectangle_i.setFillColor(sf::Color::Red);
    sf::RectangleShape rectangle_r(sf::Vector2f(1.f, 1.f));
    rectangle_r.setFillColor(sf::Color::Blue);
    sf::CircleShape graph_s(1.f);
    graph_s.setFillColor(sf::Color::Green);
    sf::CircleShape graph_i(1.f);
    graph_i.setFillColor(sf::Color::Red);
    sf::CircleShape graph_r(1.f);
    graph_r.setFillColor(sf::Color::Blue);
    int day = 0;
    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }
        int const s = counter(board, State::Susceptible);
        int const i = counter(board, State::Infected);
        int const r = counter(board, State::Recovered);
        assert(s + i + r == number_of_people);
        std::cout << "at day " << day << ", the number of the population is " << p0.S + p0.I + p0.R << '\n';
        std::cout << "Expected parameters are: " << '\n';
        std::cout << "R0: " << R0 << ", Beta: " << Beta << ", Gamma: " << gamma_ << '\n';
        std::cout << "Expected S: " << p0.expected_S << '\n';
        std::cout << "Expected I: " << p0.expected_I << '\n';
        std::cout << "Expected R: " << p0.expected_R << '\n';
        std::cout << "Obtained parameters are: " << '\n';
        std::cout << "R0: " << p0.R0_ << ", Beta: " << p0.Beta << ", Gamma: " << p0.gamma_ << '\n';
        std::cout << "Obtained S: " << s << '\n';
        std::cout << "Obtained I: " << i << '\n';
        std::cout << "Obtained R: " << r << '\n';
        graph_s.setPosition(dim + day, (s / (dim - 2) ^ 2));
        window.draw(graph_s);
        graph_i.setPosition(dim + day, (i / (dim - 2) ^ 2));
        window.draw(graph_i);
        graph_r.setPosition(dim + day, ((r / (dim^ 2))));
        window.draw(graph_r);
        for (int i = 0; i != board.size(); ++i) {
            for (int j = 0; j != board.size(); ++j) {
                if (board(i, j) == State::Susceptible) {
                    rectangle_s.setPosition(j, i);
                    window.draw(rectangle_s);
                }
                if (board(i, j) == State::Infected) {
                    rectangle_i.setPosition(j, i);
                    window.draw(rectangle_i);
                }
                if (board(i, j) == State::Recovered) {
                    rectangle_r.setPosition(j, i);
                    window.draw(rectangle_r);
                }
            }
        }
        window.display();
        p0 = covid.evolve(p0);
        evolve(board, p0);
        move(board);
        if (i == 0) {
            window.close();
        }
        day = day + 1;
    }
 
}
*/