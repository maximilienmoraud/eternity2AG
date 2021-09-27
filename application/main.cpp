#include <iostream>
#include "initSolution.h"
#include "evalSolution.h"
#include "swapRotate.h"
#include "crossContourCentre.h"
#include <crossOrderXover.h>
#include "localSearch.h"
#include "swapLocalSearch.h"
#include <eoSecondsElapsedContinue.h>
#include <eo>
#include <sys/stat.h>
#include <swapGoodSquare.h>

#include <string>
#include <sstream>

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}



int main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){

    //prise du temps de départ pour connaitre le temps d'execution
    clock_t tStart = clock();

    //init taille bar de suivit
    int barWidth = 70;

    //init des variables du problème
    eoParser parser(argc, argv);
    unsigned int tailleGen = parser.createParam((unsigned int)(10), "tailleGen", "Taille de la generation", 'g', "Param").value();
    bool LSactive = parser.createParam((bool)(false), "LSactive", "LS actif ?", 'L', "Param").value();
    unsigned int nbLS = parser.createParam((unsigned int)(1), "nombreLS", "Nombre de Local Search", 'l', "Param").value();
    unsigned int maxGenAG = parser.createParam((unsigned int)(100), "maxGenAG", "Nombre max d'AG", 'M', "Param").value();
    unsigned int increaseObj = parser.createParam((unsigned int)(5), "increaseObj", "Nombre Objectif de gain fitness par AG", 'O', "Param").value();
    unsigned int strategieInit = parser.createParam((unsigned int)(3), "strategieInit", "Strategie initialisation", 'I', "Param").value();
    unsigned int nbLigneMut = parser.createParam((unsigned int)(3), "nbLigneMut", "Nombre de ligne affecté par la mutation", 'M', "Param").value();
    float tauxCross = parser.createParam((float)(0.5), "tauxCross", "Taux de cross", 'm', "Param").value();
    float tauxSwap = parser.createParam((float)(0.5), "tauxSwap", "Taux de swap", 's', "Param").value();
    std::string user = "M.MORAUD & A.WATERS";

    // creation des fichiers de suivi
    std::string temp = "../../essai/" + ToString(tailleGen) + "_" + ToString(tailleGen) + "_" + ToString(LSactive) + "_" + ToString(nbLS) + "_" + ToString(maxGenAG) + "_" + ToString(strategieInit) + "_" + ToString(nbLigneMut) + "_" + ToString(tauxCross) + "_" + ToString(tauxSwap);
    char buffer [80];
    strcpy(buffer, temp.c_str());
    std::string folderName = buffer;
    mkdir(buffer,0777);
    std:: string templogName = folderName + "/log.log";
    std:: string tempmapName = folderName + "/map.txt";
    std::string tempinfoName = folderName + "/info.txt";
    char logName [80];
    strcpy(logName, templogName.c_str());
    char mapName [80];
    strcpy(mapName, tempmapName.c_str());
    char infoName [80];
    strcpy(infoName, tempinfoName.c_str());
    std::ofstream file;
    file.open(infoName);

    //chargement du problème
	Problem problem;
	problem.load("../../benchs/benchEternity2.txt");


    //init du problème
    initSolution init(problem,strategieInit); //initSolution with a strategy
    evalSolution eval(problem);

    //init local search
    localSearch ls(problem,0);

    //selection des strats d'AG
    //swapRotate mut(problem);
    //swapLocalSearch mut(problem);
    swapGoodSquare mut(problem);
    crossOrderXover cross(problem, nbLigneMut);
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
    eoSGATransform<Solution> transform(cross, tauxCross, mut, tauxSwap);
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
        if (LSactive) {
            for (int j = 0; j < tailleGen; ++j) {
                ls(pop[j]);
            }
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
