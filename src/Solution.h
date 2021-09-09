#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <eo>
//#include <eoVector.h>
#include <eoScalarFitness.h>
#include "Piece.h"

class Solution: public EO<eoMinimizingFitness>, public std::vector <struct Piece>{
 public:
  void printOn(std::ostream& os) const;
  void readFrom(std::istream& is);

  Solution(const Solution&); 
  Solution();
  Solution& operator=(const Solution&);

};

#endif
