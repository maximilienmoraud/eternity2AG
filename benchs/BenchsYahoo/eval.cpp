#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>

int main(int argc, char* argv[]){ 
  assert(argc==3);
  
  std::vector<std::vector<unsigned int> > pieces;
  std::vector<std::pair<unsigned int, unsigned int> > solution;
  unsigned int h; //hauteur
  unsigned int l; //largeur
  
  unsigned int result=0;
  
  std::ifstream bench(argv[1], std::ios::in);
  std::ifstream reponse(argv[2], std::ios::in);
  
  //Load bench
  if(!bench.is_open())
    std::cout << "Benchmarck file cannot be open!!!" << std::endl;
  else{
    bench >> l >> h;
    unsigned int down=0;
    unsigned int left=0;
    unsigned int up=0;
    unsigned int right=0;
    pieces.resize(l*h);
    for(unsigned int i=0; i<(l*h); i++){
      bench >> down >> left >> up >> right;
      pieces[i].push_back(down);
      pieces[i].push_back(left);
      pieces[i].push_back(up);
      pieces[i].push_back(right);
    }
  }
  
  //Print pieces
  
  for(unsigned int i=0; i<(l*h); i++){
    std::cout << "Piece " << i << ": " << pieces[i][0] << " " << pieces[i][1] << " " << pieces[i][2] << " " << pieces[i][3] << std::endl;
  }
  std::cout << std::endl;
  
  //Load answer
  if(!reponse.is_open())
    std::cout << "Answer file cannot be open!!!" << std::endl;
  else{
    std::pair<unsigned int, unsigned int> tmp;
    for(unsigned int i=0; i<(l*h); i++){
      reponse >> tmp.first >> tmp.second;
      solution.push_back(tmp);
    }
  }
  
  //Print reponse
  std::cout << "Solution:" << std::endl;
  std::cout << "---------" << std::endl;
  for(unsigned int i=0; i<(l*h); i++){
    if(i<10)
      std::cout << "00" << i << " -> " << solution[i].first << " " << solution[i].second << std::endl;
    else if(i<100)
      std::cout << "0" << i << " -> " << solution[i].first << " " << solution[i].second << std::endl;
    else
      std::cout << i << " -> " << solution[i].first << " " << solution[i].second << std::endl;
  }
  
  //Verif if solution is valid
  //verif top border
  for(unsigned int i=0; i<l; i++){
    if(pieces[solution[i].first][(6-solution[i].second)%4]!=0){
        std::cout << "Top border -> INVALID" << std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout << "Top border -> VALID" << std::endl;

  //verif bottom border
  for(unsigned int i=l*(h-1); i<l*h; i++){
    if(pieces[solution[i].first][(4-solution[i].second)%4]!=0){
  std::cout << "Bottom border -> INVALID" << std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout << "Bottom border -> VALID" << std::endl;

  //verif left border
  for(unsigned int i=0; i<l*h; i+=l){
    if(pieces[solution[i].first][(5-solution[i].second)%4]!=0){
      std::cout << "Left border -> INVALID" << std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout << "Left border -> VALID" << std::endl;

  //verif right border
  for(unsigned int i=l-1; i<l*h; i+=l){
    if(pieces[solution[i].first][(3-solution[i].second)%4]!=0){
      std::cout << "Right border -> INVALID" << std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout << "Right border -> VALID" << std::endl << std::endl;


  std::cout << std::endl;
  std::cout << "Verif horizontal:" << std::endl;
  std::cout << "-----------------" << std::endl;
  //Evaluates
  for(unsigned int i=0; i<(l*h-1); i++){
    if( (i%l) != (l-1)){
      std::cout << i << ", " << i+1;
      if(pieces[solution[i].first][(3-solution[i].second)%4] == pieces[solution[i+1].first][(5-solution[i+1].second)%4]){
	result++;
	std::cout << " -> ok" << std::endl;
      }
      else
	std::cout << " -> not ok" << std::endl;
    }
  }
  
  std::cout << std::endl;
  std::cout << "Verif veritcal:" << std::endl;
  std::cout << "---------------" << std::endl;

  for(unsigned int i=0; i<(l*(h-1)); i++){
    std::cout << i << ", " << i+l;
    if(pieces[solution[i].first][(4-solution[i].second)%4] == pieces[solution[i+l].first][(6-solution[i+l].second)%4]){
      result++;
      std::cout << " -> ok" << std::endl;
    }
    else
      std::cout << " -> not ok" << std::endl;
  }
  
  std::cout << std::endl << "Fitness: " << result << std::endl; 
  
  return EXIT_SUCCESS;

}
