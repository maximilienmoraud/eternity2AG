#ifndef EVALSOLUTION_H_
#define EVALSOLUTION_H_

#include "Solution.h"
#include "Problem.h"
#include "eoEvalFunc.h"
#include <cstring>

class evalSolution : public eoEvalFunc <Solution> {

 public :
  explicit evalSolution(Problem & _problem);

  void operator()(Solution& _sol);
	
 private:
	Problem & problem;
	
};

#endif 
