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
#include <swapGoodSquare.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){

    //prise du temps de départ pour connaitre le temps d'execution
    clock_t tStart = clock();

    //init taille bar de suivit
    int barWidth = 70;

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
    int nbLS = 40;
    int maxGenAG = 1000;
    int increaseObj = 5;
    std::string user = "M.MORAUD";


    //chargement du problème
	Problem problem;
	problem.load("../../benchs/benchEternity2.txt");


    //init du problème
    initSolution init(problem,2); //initSolution with a strategy
    evalSolution eval(problem);

    //init local search
    localSearch ls(problem,0);

    //selection des strats d'AG
    //swapRotate mut(problem);
    //swapLocalSearch mut(problem);
    swapGoodSquare mut(problem);
    crossOrderXover cross(problem, 3);
    //crossContourCentre cross(problem);

    //init methode de remplacement
    eoGenerationalReplacement<Solution> genReplace;
    eoWeakElitistReplacement<Solution> replace(genReplace);


    //init methode d'evaluation
    eoEvalFuncCounter<Solution> evalFunc(eval);
    eoPopLoopEval<Solution> popEval(evalFunc);

    //init du tournois
    eoDetTournamentSelect<Solution> tournament(20);
    eoSelectMany<Solution> select(tournament, 1);

    //init des derniers parametres
    eoSGATransform<Solution> transform(cross, 0.3, mut, 0.7);
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
    std::cout << "[" << ((clock() - tStart)/CLOCKS_PER_SEC) <<"s] Lauching generation fist pop ... " << std::endl;
    for(unsigned int i=0; i<tailleGen; i++){
        //affichage de l'avancement
        float tempi = i;
        float tempTailleGen = tailleGen;
        float progress = (tempi)/tempTailleGen;
        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();

        init(s);
        eval(s);
        pop.push_back(s);


    }
    std::cout << "[";
    for (int i = 0; i < 70; ++i) {
        std::cout << "=";
    }
    std::cout << "] " << int(100.0) << " %\r";
    std::cout << std::endl;
    std::cout << "[" << ((clock() - tStart)/CLOCKS_PER_SEC) <<"s] Fist pop generated, best fit of generation is : " << pop.best_element().fitness() << std::endl;

    //Editing log file
    if (file.is_open()){
        file << "[" << (clock() - tStart)/100000 << "ms]  " << "First generation created, best fit of generation is : " << std::endl << pop.best_element().fitness() << std::endl;
    }
    std::cout << "[" << ((clock() - tStart)/CLOCKS_PER_SEC) <<"s] Lauching AGxLS ... " << std::endl;
    //boucle de déclanchement de la LS
    for (int i = 0; i < nbLS; ++i) {
        //affichage de l'avancement
        float tempi = i;
        float tempNbLS = nbLS;
        float progress = (tempi)/tempNbLS;
        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();

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

        // execution de l'algo
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "Lauching AG ["<< i + 1 <<"] ... " << std::endl;
        }
        algo(pop);
        if (file.is_open()) {
            file << "[" << (clock() - tStart)/100000 << "ms]  " << "New best fit : " << std::endl << pop.best_element().fitness() << std::endl;
        }

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


    }
    std::cout << "[";
    for (int i = 0; i < 70; ++i) {
        std::cout << "=";
    }
    std::cout << "] " << int(100.0) << " %\r";
    std::cout << std::endl;

    //ajout du temps d'execution dans le fichier
    file.close();

    //affichage de la best solution
    std::cout << "[" << ((clock() - tStart)/CLOCKS_PER_SEC) <<"s] AGxLS finished, best FIT : " << pop.best_element().fitness() << std::endl;

    //sauvegarde de la best solution
    problem.printSolinFile(pop.best_element(), "../../../sketch_190409a/positions.txt");
    problem.printSolinFile(pop.best_element(), mapName);

    return 0;
}
