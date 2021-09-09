#ifndef INITSOLUTION_H_
#define INITSOLUTION_H_

#include "Solution.h"
#include "Problem.h"
#include "utils/eoRNG.h"
#include <eoInit.h>
#include <cstring>
#include <map>



class initSolution : public eoInit <Solution> {

 public :
  initSolution(Problem & _problem, unsigned int _strategie);

  void setStrategie(unsigned int _strategie);
  
  void operator()(Solution& _sol);

  void initAleatoire(Solution& _sol);
  void initGlouton(Solution& _sol);
	
 private:
	Problem & problem;
	unsigned int strategie;
	
};

#endif 
