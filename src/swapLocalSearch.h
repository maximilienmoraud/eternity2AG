#include "Solution.h"
#include "Problem.h"
#include "localSearch.h"
#include "utils/eoRNG.h"
#include "eoOp.h"

class swapLocalSearch: public eoMonOp<Solution>
{
public:

    swapLocalSearch(Problem& _problem):problem(_problem){}

    bool operator()(Solution& _sol){
        // fonction de swap qui appel une methode de LS
        localSearch ls(problem,0);

        ls(_sol);

        _sol.invalidate();
        return true;
    }


private:
    Problem& problem;
};
