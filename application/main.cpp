#include <iostream>
#include "Piece.h"
#include "initSolution.h"
#include "evalSolution.h"
#include "swapRotate.h"
#include "crossContourCentre.h"
#include <chrono>
#include <thread>
#include<unistd.h>


#include <eo>

int main(int argc, char** argv){

	Problem problem;
	problem.load("/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/Code/benchs/benchEternity2.txt");


	double bestScore = 0;
	double mean = 0;
	unsigned int iteration = 10000;
	for(unsigned int i=0; i<iteration; i++){
	    initSolution init(problem,1); //initSolution with a strategy
	    evalSolution eval(problem);
	    Solution s;
	    init(s);
	    eval(s);
	    //problem.printSol(s);
	    //problem.printSolinFile(s, "/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/sketch_190409a/positions.txt");
	    //std::cout << "Score: " << s.fitness() << std::endl;

	    mean = mean + s.fitness();

	    if (bestScore > s.fitness()){
	        bestScore = s.fitness();
	    }
	    //unsigned int microsecond = 1000000;
	    //usleep(1 * microsecond);//sleeps for 3 second
	}
	std::cout << "Best Score: " << bestScore << std::endl;
	std::cout << "Mean Score: " << mean/iteration << std::endl;

  return 0;
}
