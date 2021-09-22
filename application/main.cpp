#include <iostream>
#include "initSolution.h"
#include "evalSolution.h"
#include "swapRotate.h"
#include "crossContourCentre.h"
#include <chrono>
#include <thread>
#include <crossOrderXover.h>
#include "localSearch.h"
#include "swapLocalSearch.h"
#include <eoSecondsElapsedContinue.h>
#include <eo>
#include <sys/stat.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){

    //prise du temps de départ pour connaitre le temps d'execution
    clock_t tStart = clock();

    // creation des fichiers de suivi
    time_t t = time(nullptr);
    struct tm * now = localtime( & t );
    char buffer [80];
    strftime (buffer,80,"../../essai/%d-%m-%Y_%H-%M-%S",now);
    std::string folderName = buffer;
    mkdir(buffer,0777);
    std:: string logName = folderName + "/log.log";
    std:: string mapName = folderName + "/map.txt";
    std::string infoName = folderName + "/info.txt";
    std::ofstream file;
    file.open(infoName);

    //init des variables du problème
    int tailleGen = 100;
    int nbLS = 30;
    int maxGenAG = 250;
    int increaseObj = 5;
    std::string user = "M.MORAUD";


    //chargement du problème
	Problem problem;
	problem.load("/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/Code/benchs/benchEternity2.txt");


    //init du problème
    initSolution init(problem,2); //initSolution with a strategy
    evalSolution eval(problem);

    //init local search
    localSearch ls(problem,0);

    //selection des strats d'AG
    swapRotate mut(problem);
        //swapLocalSearch mut(problem);
    crossOrderXover cross(problem, 2);
        //crossContourCentre cross(problem);

    //init methode de remplacement
    eoGenerationalReplacement<Solution> genReplace;
    eoWeakElitistReplacement<Solution> replace(genReplace);

    //init methode d'evaluation
    eoEvalFuncCounter<Solution> evalFunc(eval);
    eoPopLoopEval<Solution> popEval(evalFunc);

    //init du tournois
    eoDetTournamentSelect<Solution> tournament(15);
    eoSelectMany<Solution> select(tournament, 1);

    //init des derniers parametres
    eoSGATransform<Solution> transform(cross, 0, mut, 0.5);
    eoSelectTransform<Solution> breed(select, transform);

    //init de la pop
    eoPop<Solution> pop;
    Solution s;

    //Editing log file
    if (file.is_open()) {
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "Log file created by "<< user << std::endl;
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "Generation size is : " << tailleGen << std::endl;
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "Nb LS is : " << nbLS << std::endl;
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "Maximum generation per AG : " << maxGenAG << std::endl;
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "Trying to increase each AG : " << increaseObj << std::endl;
        file << "[" << (clock() - tStart) / 100000 << "ms]  " << "Lauching genereation first pop " << std::endl;
    }

    //remplissage de la population
    for(unsigned int i=0; i<tailleGen; i++){
        init(s);
        eval(s);
        pop.push_back(s);
    }

    //Editing log file
    if (file.is_open()){
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "First generation created, best fit of generation is : " << std::endl << pop.best_element().fitness() << std::endl;
    }

    //boucle de déclanchement de la LS
    for (int i = 0; i < nbLS; ++i) {

        //init critère de continuation
        eoGenContinue<Solution> genCont1(maxGenAG);
        eoFitContinue<Solution> genCont2(pop.best_element().fitness()+increaseObj);
        eoCombinedContinue<Solution> genCont(genCont1,genCont2);
        eoCheckPoint<Solution> cp(genCont1);

        //init remplissage des logs
        eoFileMonitor fileMonitor(logName, " ", true);
        eoBestFitnessStat<Solution> bestStat;
        cp.add(bestStat);
        fileMonitor.add(bestStat);
        cp.add(fileMonitor);

        //init algo
        eoEasyEA<Solution> algo(cp, popEval, breed, genReplace);

        //boucle LS
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "Lauching LS ["<< i + 1 <<"] ... " << std::endl;
        }
        for (int j = 0; j < tailleGen; ++j) {
            ls(pop[j]);
        }
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "New best fit : " << std::endl << pop.best_element().fitness() << std::endl;
        }

        // execution de l'algo
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "Lauching AG ["<< i + 1 <<"] ... " << std::endl;
        }
        algo(pop);
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "New best fit : " << std::endl << pop.best_element().fitness() << std::endl;
        }

        //affichage de l'avancement
        float tmpI = i;
        float tmpNbTour = nbLS;
        std::cout << "Avancement ["<< ((tmpI+1)/tmpNbTour)*100 <<"%] Execution time [" << ((clock() - tStart)/CLOCKS_PER_SEC) <<"s]" << std::endl;
    }
    //ajout du temps d'execution dans le fichier
    file.close();

    //affichage de la best solution
    std::cout << "Best FIT : " << pop.best_element().fitness() << std::endl;

    //sauvegarde de la best solution
    problem.printSolinFile(pop.best_element(), "/Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ETERNITY2/sketch_190409a/positions.txt");
    problem.printSolinFile(pop.best_element(), mapName);

    return 0;
}
