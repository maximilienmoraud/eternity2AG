#include <iostream>
#include "Piece.h"
#include "initSolution.h"
#include "evalSolution.h"
#include "swapRotate.h"
#include "crossContourCentre.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <crossOrderXover.h>
#include "localSearch.h"
#include "swapLocalSearch.h"

#include <eo>
#include <random>

int main(int argc, char** argv){
	Problem problem;
	problem.load("/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/Code/benchs/benchEternity2.txt");

    initSolution init(problem,1); //initSolution with a strategy
    evalSolution eval(problem);
    localSearch ls(problem,0);

    //swapRotate mut(problem);
    swapLocalSearch mut(problem);
    crossOrderXover cross(problem, 2);
    //crossContourCentre cross(problem);

    eoGenerationalReplacement<Solution> genReplace;
    eoWeakElitistReplacement<Solution> replace(genReplace);

    eoEvalFuncCounter<Solution> evalFunc(eval);
    eoPopLoopEval<Solution> popEval(evalFunc);

    eoDetTournamentSelect<Solution> tournament(10);

    eoSelectMany<Solution> select(tournament, 1);

    eoSGATransform<Solution> transform(cross, 0.05, mut, 0.5);

    eoSelectTransform<Solution> breed(select, transform);

    eoPop<Solution> pop;
    Solution s;


    srand(time(0));
    int tailleGen = 100;
    for(unsigned int i=0; i<tailleGen; i++){
        init(s);
        eval(s);


        //problem.printSol(s);
        //std::cout << "Score: " << s.fitness() << std::endl;
        pop.push_back(s);
    }


    eoGenContinue<Solution> genCont(200);
    eoCheckPoint<Solution> cp(genCont);
    //cp.add(bestStat);
    eoFileMonitor fileMonitor("stats.xg", " ");
    eoBestFitnessStat<Solution> bestStat;
    cp.add(bestStat);
    fileMonitor.add(bestStat);
    cp.add(fileMonitor);

    // eoSteadyFitContinue<Solution> genCont(1,0);

    eoEasyEA<Solution> algo(cp, popEval, breed, genReplace);

    algo(pop);

    //problem.printSol(pop.best_element());
    std::cout << "Best FIT : " << pop.best_element().fitness() << std::endl;


/*
    Solution s;
    init(s);
    eval(s);
    std::cout << "Score: " << s.fitness() << std::endl;
    ls(s);
    std::cout << "Score: " << s.fitness() << std::endl;
    */
    problem.printSolinFile(pop.best_element(), "/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/sketch_190409a/positions.txt");


    return 0;
}
