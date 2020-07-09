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


using namespace std::chrono_literals;

constexpr double gamma_ = 0.0351;
constexpr double R0 = 4.58;
constexpr double Beta = 0.151;

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
 double expected_s;
 double expected_i;
 double expected_r;
 int expected_S;
 int expected_I;
 int expected_R;
};

class epidemy {
 Population m_p0;
 int m_number_of_people;

public: 
 epidemy(Population p0, int const& number_of_people) : m_p0{p0}, m_number_of_people{number_of_people} {};

auto evolve(int const& duration, Population const& prev) const 
 { 
   Population p;
   //p.expected_s = prev.s - p.R0_ * p.gamma_ * prev.s * prev.i ;
   //p.expected_i = prev.i + (p.R0_ * prev.s - 1) * p.gamma_ * prev.i ;
   //p.expected_r = prev.expected_r + gamma_ * prev.i ;
   
   
   p.s = prev.s - prev.R0_ * prev.gamma_ * prev.s * prev.i ;
   p.i = prev.i + (prev.R0_ * prev.s - 1) * prev.gamma_ * prev.i ;
   p.r = prev.r + prev.gamma_ * prev.i ;
   p.expected_s = prev.expected_s - R0 * gamma_ * prev.expected_s * prev.expected_i ;
   p.expected_i = prev.expected_i + (R0 * prev.expected_s - 1) * gamma_ * prev.expected_i ;
   p.expected_r = prev.expected_r + gamma_ * prev.expected_i ;
   double sum = std::round(p.i + p.s + p.r);
   assert(sum == 1); 
   p.R0_ = ( 1 + (p.i - prev.i) / (p.r - prev.r) )  /  p.s;
   p.Beta = ( (p.r - prev.r) + (p.i - prev.i) ) / ( p.s  * p.i );
   p.gamma_ = ( (p.r - prev.r) / p.i );
      
   //formule per quando si avranno cambi di comportamento da parte della popolazione

   p.I = std::round(p.i * m_number_of_people);
   p.R = std::round(p.r * m_number_of_people);
   p.S = m_number_of_people - p.I - p.R;
   p.expected_I = std::round(p.expected_i * m_number_of_people);
   p.expected_R = std::round(p.expected_r * m_number_of_people);
   p.expected_S = m_number_of_people - p.expected_I - p.expected_R;
   return p;
 }
};

void printPopulation (Population const& pop, int const& day) {
 std::cout << "Day  ||    S   ||    I    ||    R\n";

  std::cout<< "giorno " << day + 1 << "  " << pop.S << " , " << pop.I << " , " <<pop.R 
  << " "<< pop.s << " , " << pop.i << " , " <<pop.r<< '\n';
  }


auto r_limit () { //inserire parametri variabili
  std::vector<double> rtest (1000);
std::generate(rtest.begin(), rtest.end(),[i=0.]()mutable //fill rtest
  {
   return i += 0.001;
  } ); 
for (auto& x : rtest) //computing the deficit
  {
    x = std::abs( x - 1 + std::exp(-R0 * x) );
  }
  auto first = rtest.begin();
  auto minimum_it = min_element (rtest.begin(),rtest.end());
  double const r_min_value = ( std::distance(first, minimum_it) ) * 0.001;
  return r_min_value;
}

class Board {
    int n_;
    std::vector<std::vector<State>> board_;
public:
    Board(int n) : n_{ n }, board_(n+2, std::vector<State>(n+2, State::R)) 
    {assert(n>2);}

    State operator() (int i, int j) const {
        return (i >= 1 && i < (n_ + 1) && j >= 1 && j < (n_ + 1)) ? board_[i][j]
            : State::R;
        
    }
    State& operator() (int i, int j) {
        //assert(i >= 1 && i < (n_ - 1) && j >= 1 && j < (n_ - 1));
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

void Initialize(Board& board, double const probability){
    for (int i = 0; i != (board.size() + 1); ++i) {
    for (int j = 0; j != (board.size() + 1); ++j) {
        if (dis(gen) < probability) {
        board(i,j) = State::I;
        } else {board(i,j) = State::S;}
    }
    } 

}


void evolve(Board& board, Population const& population) 
{
    double const probability_infection = population.i;
    double const probability_recovered = population.r;
    for(int r = 1; r != board.size()+1; ++r){
     for(int c = 1; c != board.size()+1; ++c){
         if (board(r,c) == State::I){
          for(int i = r-1; i != r+1; ++i){
           for(int j = c-1; j != c+1; ++j){
             if (board(i,j) == State::S){
              if (dis(gen) < probability_infection){
               board(i,j) = State::I; 
             }
            }  
           }
          }
          if(dis(gen) < probability_recovered){
              board(r,c) = State::R;
          }    
         }
     }
    }
}

int estraction(int const& lower, int const& upper) {
std::uniform_int_distribution<> dis(lower, upper);
return dis(gen);
}
/*
template<typename T> void swap(T& t1, T& t2) {
    T temp = std::move(t1); // or T temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}*/

void move(Board& board){
    for(int r = 2; r != board.size(); ++r){
     for(int c = 2; c != board.size(); ++c){
       std::swap(board(r,c),board( estraction(r-1, r+1), estraction(c-1, c+1)));//1 = range
     }
    }
}


int counter(Board const& board, State state) {
  int number = 0;
  int const N = board.size();
  for (int i=1; i!=N+1; i++) {
      for(int j=1; j !=N+1; j++){
      if (board(i,j) == state) {
      number = number + 1;
     }
    }
  }
  return number;
}




int main() {
const int total_days = 60;
const int number_of_people = 144;
Board board(12);
Population p0{};
p0.i=0.04;
p0.s=0.96;
p0.expected_i=0.04;
p0.expected_s=0.96;
p0.Beta = Beta;
p0.gamma_ = gamma_;
p0.R0_ = R0;
epidemy covid(p0,number_of_people);
std::cout << "% of people involved in epidemy is: " << 
std::round(r_limit())<<'\n';
std::cout << "% of people not involved in epidemy is: " << 
1 - std::round(r_limit()) <<'\n';     
for (int day=1; day<=total_days; day++) {
p0 = covid.evolve(total_days, p0);
if(day==1){
    Initialize(board, p0.i);
    print(board);
    }

evolve(board, p0);
//move(board);
print(board);
int const s = counter(board, State::S);
int const i = counter(board, State::I);
int const r = counter(board, State::R);
assert (s+i+r == number_of_people );
std::cout << "at day " << day << ", the number of the population is " << p0.S+p0.I+p0.R << '\n';
std::cout << "Expected parameters are: " <<'\n';
std::cout << "R0: " << R0 << ", Beta: " << Beta << ", Gamma: " << gamma_ << '\n';  
std::cout << "Expected S: " << p0.expected_S <<'\n';
std::cout << "Expected I: " << p0.expected_I <<'\n';
std::cout << "Expected R: " << p0.expected_R <<'\n';    
std::cout << "Obtained parameters are: " <<'\n';
std::cout << "R0: " << p0.R0_ << ", Beta: " << p0.Beta << ", Gamma: " << p0.gamma_ <<'\n';
std::cout << "Obtained S: " << s <<'\n';
std::cout << "Obtained I: " << i <<'\n';
std::cout << "Obtained R: " << r <<'\n';
std::this_thread::sleep_for (std::chrono::milliseconds(500));
}
/*int dim = 200;
    Board board(dim);
    Population p0{};
    p0.i=0.04;
    p0.s=0.96;
    epidemy covid(p0,number_of_people);
    Initialize(board);
    sf::RenderWindow window(sf::VideoMode(dim, dim), "Simulation of an epidemic");
    sf::RectangleShape rectangle_s(sf::Vector2f(1.f, 1.f));
    rectangle_s.setFillColor(sf::Color::Green);
    sf::RectangleShape rectangle_i(sf::Vector2f(1.f, 1.f));
    rectangle_i.setFillColor(sf::Color::Red);
    sf::RectangleShape rectangle_r(sf::Vector2f(1.f, 1.f));
    rectangle_r.setFillColor(sf::Color::Blue);
    std::cout << "Number of Susceptible, Infectious and Recovered per day\n";
    std::cout << "Susceptible " << " Infectious " << " Recovered " << " tot\n";
    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }
        int S = counter(board,State::S);
        int I = counter(board,State::I);
        int R = counter(board,State::R);
        int T = S + I + R;
        std::cout << S << "        " << I << "          " << R << "            " << T << '\n';
        for (int i = 0; i != board.size(); ++i) {
             for (int  j = 0; j != board.size(); ++j) {
                    if (board(i, j) == State::S) {
                        rectangle_s.setPosition(j, i);
                        window.draw(rectangle_s);
                    }
                    if (board(i, j) == State::I) {
                        rectangle_i.setPosition(j, i);
                        window.draw(rectangle_i);
                    }
                    if (board(i, j) == State::R) {
                        rectangle_r.setPosition(j, i);
                        window.draw(rectangle_r);
                    }
             }
        }
        window.display();
        evolve(board,p0);
        move(board);
        if (I == 0) {
            window.close();
        }

    }*/
}