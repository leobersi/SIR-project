#include <iostream>
#include <cassert>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>
#include <thread>         // per std::this_thread::sleep_for
#include <chrono>         // per i secondi scritti "s"

constexpr double gamma_ = 0.0351;
constexpr double R0 = 4.58;

enum class State : char { S, I, R };//prova ad implementare enum class

class Person {
 State m_state;
 int m_prob_value;
 int m_coloumn;
 int m_row;
 int m_I_days_counter;
public:
 Person () : m_state{State::S}, m_prob_value{0}, m_coloumn{0}, m_row{0}, m_I_days_counter{0} {}  
 void setstate (State state) { 
  m_state = state; 
 }
 void incrementprob_value () {
  ++m_prob_value ;
 }
 void setcol (int c) { 
  m_coloumn = c; 
 }
 void setrow (int r) { 
  m_row = r; 
 }
 void days_counter () {
  ++m_I_days_counter;
 }
 State const& getstate() {
  return m_state;
 }
 int const& getprob_value() {
  return m_prob_value;
 }
 int const& getcol () {
  return m_coloumn;
 }
 int const& getrow () {
  return m_row;
 }
 int const& getIdays_counter () {
  return m_I_days_counter;
 }
};



class Board {
  int n_;
  std::vector<std::vector<Person>> board_;

 public:
  Board(int& n) : 
  n_{ n } ,  
  board_( n + 2 , std::vector<Person>(n + 2)) 
  {assert(n_ > 2);}


/*Person& operator()(int const& i, int const& j) {
 return board_[i][j];
}*/

Person operator()(int const& i, int const& j) const {
 assert (i>0 && i<(n_+2) && j>0 && j<(n_+2));
 return board_[i][j];
}
int size() const {return n_;}
};

//potrei aggiungere un metodo remove delle caselle per implementare i tamponi

struct Population{
 double s;
 double i;
 double r;
};
////////////////////////////////////////////////////////////////////////
//stuff with Initialize
int randrange(int const& low,int const& high) {
    return rand()%(high-low+1)+low;     
}

/*struct Estraction {
 int row;
 int coloumn;
};*/

Board Initialize(Board const& board, Population const& p0, int const& number_of_people)
{
 int N = board.size();
 Board r_board(N); 
 for(int i=0; i < N+2 ;++i) { //riempimento matrice interna ed esterna
  for(int j=0; j < N+2 ;++j) {
   if (i>0 && i<(N+1) && j>0 && j<(N+1)) 
   {
    r_board(i,j).setstate(State::S);
   } else { r_board(i,j).setstate(State::R); }
  }
 }
 //generic position of a number p0.i of Infects
 const int number_of_I0 = static_cast<int>( std::round(p0.i * number_of_people) );
 std::map<int,int> control; 
 int c,r;
 for(int i=0; i<number_of_I0; i++) {
  do { 
   c = randrange(1,N); 
   r = randrange(1,N);           
  } while(control.find(c)!=control.end()); //assert here
 control.insert ( std::pair<int,int>(c,r) );
 }
 for (auto it=control.begin(); it!=control.end(); ++it)
 {
  r_board( (it->first), (it->second) ).setstate(State::I);
 }
 return r_board;
} 
////////////////////////////////////////////////////////////////////////


class epidemy {
 Population m_p0;

public: 
 epidemy(Population p0) : m_p0{p0} {};

std::vector<Population> evolve(int const& duration) const 
{ 
std::vector<Population> result{m_p0};
 for (int day =1; day < duration; day++) 
  {
   auto& prev= result.back();
   Population p;
   p.s = prev.s - R0 * gamma_ * prev.s * prev.i ;
   p.i = prev.i + (R0 * prev.s - 1) * gamma_ * prev.i ;
   p.r = prev.r + gamma_ * prev.i ;
   double sum = std::round(p.i + p.s + p.r);
   assert(sum == 1);
   result.push_back(p);
  }
return result;
}
};

void printPopulation (std::vector<Population> sir) {
 std::cout << "Day  ||    S   ||    I    ||    R\n";
 int i=1;
 for (auto& v : sir) {
    
  std::cout<< "giorno " << i << "  " << v.s << " , " << v.i << " , " << v.r <<'\n';
  ++i;
  }
}


auto r_limit () {
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



auto probability_evolve(Board const& board) {      
 std::vector<Person> candidates;
 std::vector<Person> older_I;
 std::pair <std::vector<Person>, std::vector<Person>> result;
 int N = board.size();
 for(int c=1; c < N+1 ;++c) { 
 for(int r=1; r < N+1 ;++r) {
  if ((board(c,r).getstate()) == State::I) { //assert che non ci siano I nella cornice esterna
   board(c,r).days_counter(); //counting illness days 
   board(c,r).setcol(c);
   board(c,r).setrow(r);
   for (int i= c-1; i<c+2 ;++i) {
    for (int j=r-1; j<r+2 ;j++) {
     if (((board(c,r).getstate()) == State::S)) {
      board(i,j).incrementprob_value();
      board(i,j).setcol(i);
      board(i,j).setrow(j);
      candidates.push_back(board(i,j)); //all S around each I are in the vector   
     }
    }
   }
   older_I.push_back(board(c,r)); //all Ill-days values in the vector
  }
 }
 }
 std::sort(candidates.begin(), candidates.end(), [](Person first, Person second) { //sorting the vector
        return first.getprob_value() >= second.getprob_value();   
    });
 std::sort(older_I.begin(), older_I.end(), [](Person first, Person second) { //sorting the vector
        return first.getIdays_counter() >= second.getIdays_counter();   
    });
 result = std::make_pair(candidates, older_I);

 return result;
  }





void game(Board const& board, int const& duration, std::vector<Population> const& result) //si possono unire game ed evolve?!
{
 int N = board.size();
 //Board following_day(N);
 for (int day =2; day < duration; day++)
 {
  /*double const delta_s = ( (result.begin() + day)->s ) - 
                        ( (result.begin()+ day - 1)->s );*/
  double const delta_i = ( (result.begin() + day)->i ) - 
                        ( (result.begin()+ day - 1)->i );
  double const delta_r = ( (result.begin() + day)->r ) - 
                        ( (result.begin()+ day - 1)->r );
  int const new_I = static_cast<int>(std::round(delta_i * (result.begin() + day)->i / 100));
  int const new_R = static_cast<int>(std::round(delta_r * (result.begin() + day)->r / 100));
  //un assert per verificare che la somma I+R sia 1-S
  auto result = probability_evolve(board);
  if(new_I >= 0) {
   std::for_each ((result.first).begin(), ( (result.first).begin() ) + new_I, [&](Person a)->void {
          board(a.getcol(), a.getrow()).setstate(State::I); //modifying on board the state of the first new_I element of candidates
  });
  } 
  std::for_each ((result.second).begin(), ( (result.second).begin() ) + new_R, [&](Person a)->void {
          board(a.getcol(), a.getrow()).setstate(State::R); //modifying on board the state of the first new_I element of candidates
  });
  
  /*for(int i=0; i < N+2 ;++i) { //filling following_day
   for(int j=0; j < N+2 ;++j) {
    following_day(i,j) = board(i,j);
   }*/
  }
  std::cout << "\033[2J\033[1;1H";
   for (int i = 1; i != N+1; i++) {  //stampa la sola matrice interna, senza bordo
	  for (int j = 1; j != N+1; j++) {
	   switch (board(i,j).getstate())
     {
     case State::S:
      std::cout << "S";
      break;
     case State::I:
      std::cout << "I";
      break;
     default: 
      std::cout << "R"; 
      break;
     }    
    }
   }
  }
  }

int main() {
int n=16;      
Board board(n);
board = Initialize(board, p0);
for (int i = 0; i != 150; ++i) 
{
 board = SIR_evolution(board);
 std::this_thread::sleep_for (std::chrono::milliseconds(200));
}
}