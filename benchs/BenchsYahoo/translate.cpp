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

  unsigned int h; //hauteur
  unsigned int l; //largeur

  std::ifstream res(argv[1], std::ios::in);
  std::ofstream output(argv[2]);

  std::vector<std::pair<unsigned int, unsigned int> > sol;

  if(!res.is_open())
    std::cout << "Benchmarck file cannot be open!!!" << std::endl;
  else{
    res >> l >> h;
    sol.resize(l*h);
    unsigned int x;
    unsigned int y;
    unsigned int rotation;

    std::pair<unsigned int, unsigned int> tmp;

    for(unsigned int i=0; i<(l*h); i++){
      res >> x >> y >> rotation;
      tmp.first=i;
      tmp.second=(rotation+2)%4;
      sol[(h-1-y)*l + x]=tmp;
    }
  }
  res.close();

  for(unsigned int i=0; i<sol.size(); i++)
    output << sol[i].first << " " << sol[i].second << std::endl;

  output.close();


  return 0;
}
