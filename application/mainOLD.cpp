#include <iostream>
#include "Piece.h"
#include "initSolution.h"
#include "evalSolution.h"
#include "swapRotate.h"
#include "crossContourCentre.h"

#include <eo>


int main(int argc, char** argv){



	Problem problem;
	problem.load("/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/Code/benchs/benchEternity2.txt");

	//problem.printCoins();
	//problem.printBords();
	//problem.infoCouleur();
	//problem.print();
	//problem.printIndices();a

	initSolution init(problem,0); //initSolution with a strategy
	evalSolution eval(problem);
	swapRotate mut(problem);
	//	eoPop<Solution> pop;
	Solution s;
	for(unsigned int i=0; i<1; i++){
		init(s);
		eval(s);
		problem.printSol(s);
		std::cout << "Score: " << s.fitness() << std::endl;

		mut(s);
		eval(s);
		problem.printSol(s);
		problem.printSolinFile(s, "test");
		std::cout << "Score: " << s.fitness() << std::endl;
	}




  return 0;
}
