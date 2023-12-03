#include <cctype>
#include <iostream>
#include <optional>
#include <vector>
#include <cassert>

using namespace std;

const int RED_CUBE_MAX = 12;
const int GREEN_CUBE_MAX = 13;
const int BLUE_CUBE_MAX = 14;

struct cube_subset_t{
  int blue=0, red=0, green=0;
};


class Game{

  private:
    optional<int> m_id = optional<int>{};
    vector<cube_subset_t> m_cubes;

    cube_subset_t parse_next_subset(string &raw_game, int &i){
      string num, type;

      cube_subset_t cubes;

      for(; i < raw_game.size(); i++){
        if(isdigit(raw_game[i])) num += raw_game[i];
        else if(isalpha(raw_game[i])) type += raw_game[i];

        if(raw_game[i] == ',' || raw_game[i] == ';' || i == raw_game.size()-1){
          if(type == "blue")
            cubes.blue = stoi(num);
          else if(type == "red")
            cubes.red = stoi(num);
          else if(type == "green")
            cubes.green = stoi(num);

          num = "";
          type = "";

          if(i < raw_game.size()-1 || raw_game[i] == ';'){
            i++;
            break;
          }
        }
      }

      return cubes;
    }

  public:
    Game(string &raw_game){
      parse_game(raw_game);
    }


    void parse_game(string &raw_game){
      string id;

      int i = 0;
      for(;i < raw_game.size(); i++){
        if(isdigit(raw_game[i])) id += raw_game[i];
        if(raw_game[i] == ':') break;
      }

      m_id = stoi(id);

      while(i < raw_game.size()){

        if(raw_game[i] != ';'){
          cube_subset_t c = parse_next_subset(raw_game, i);
          m_cubes.push_back(c);
        }

      }

    }

    optional<int> id(){
      return m_id;
    }
   
    bool is_valid(){
      for(cube_subset_t &subset : m_cubes){
        if(subset.blue > BLUE_CUBE_MAX) return false;
        if(subset.green > GREEN_CUBE_MAX) return false;
        if(subset.red > RED_CUBE_MAX) return false;
      }
      return true;
    }

    int get_game_power(){
      int min_green = 0, min_blue = 0, min_red = 0;
      for(cube_subset_t &subset : m_cubes){

        min_green = max(min_green, subset.green);
        min_blue =  max(min_blue, subset.blue);
        min_red =   max(min_red, subset.red);
      }
      return min_blue*min_green*min_red;
    }
};

int main(){
  string in;
  int part1 = 0; 
  int part2 = 0;
  while(getline(cin, in)){
    Game game(in);
    if(game.is_valid()){
      part1 += game.id().value();
    }
    part2 += game.get_game_power();
  }
  cout << part1 << '\n';
  cout << part2 << '\n';

}
