#ifndef ETERNITY_LOCALSEARCH_H
#define ETERNITY_LOCALSEARCH_H

#include "Solution.h"
#include "Problem.h"
#include "eoEvalFunc.h"
#include <cstring>

class localSearch : public eoEvalFunc <Solution> {

    Problem &problem;
    unsigned int strategie;

    void stratOne(Solution &solution);

public:
    localSearch(Problem & _problem, unsigned int _strategie);

    void operator()(Solution& _sol);

    void stratTwo(Solution &_sol);
};

#endif
