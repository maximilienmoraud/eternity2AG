#include "Solution.h"
#include "Problem.h"
#include "utils/eoRNG.h"
#include "eoOp.h"

class crossContourCentre: public eoQuadOp<Solution>
{
 public:

	crossContourCentre(Problem& _problem):problem(_problem){}

	bool operator()(Solution& _sol1, Solution& _sol2){

		Solution tmp = _sol1;
		for(unsigned int i=0; i<problem.idCoins.size(); i++){
			_sol1[problem.idCoins[i]]=_sol2[problem.idCoins[i]];
		}

		for(unsigned int i=0; i<problem.idBords.size(); i++){
			_sol1[problem.idBords[i]]=_sol2[problem.idBords[i]];
		}

		for(unsigned int i=0; i<problem.idCentres.size(); i++){
			_sol2[problem.idCentres[i]]=tmp[problem.idCentres[i]];
		}

		_sol1.invalidate();
		_sol2.invalidate();
		return true;
	}

 private:
	Problem& problem;
};
