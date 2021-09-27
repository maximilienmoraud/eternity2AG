#include "localSearch.h"
#include "Solution.h"
#include "Piece.h"
#include "evalSolution.h"
#include <eo>


localSearch::localSearch(Problem & _problem, unsigned int _strategie): problem(_problem), strategie(_strategie){}

int myrandomLS (int i) {
    return std::rand()%i;
}

void localSearch::operator()(Solution & _sol){
    if(strategie==0)
        stratOne(_sol);
    else if (strategie == 1)
        stratTwo(_sol);
    else{
        std::cout << "Strategie non disponible" << std::endl;
    }

}

void localSearch::stratOne(Solution & _sol) {
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    double bestFit;

    evalSolution eval(problem);

    Solution tmp;
    Piece tmpPiece;

    unsigned int bestRotationI = 0;
    unsigned int bestRotationJ = 0;

    _sol.resize(l*h);
    tmp.resize(l*h);

    std::vector<unsigned int> pcesPermutation;

    srand(time(0));
    for(unsigned int i=0; i<(h)*(l); i++)
        pcesPermutation.push_back(i);
    std::random_shuffle(pcesPermutation.begin(),pcesPermutation.end(), myrandomLS);


    for(unsigned int i=0; i<l*h; i++) {
        //std::cout << "i : " << i << std::endl;
        if (pcesPermutation[i] > l - 1 && pcesPermutation[i] < l * h - l && pcesPermutation[i] % l != 0 && (pcesPermutation[i] + 1) % l != 0) {
            for(unsigned int j=i; j<l*h; j++) {

                if (pcesPermutation[j] > l - 1 && pcesPermutation[j] < l * h - l && pcesPermutation[j] % l != 0 && (pcesPermutation[j] + 1) % l != 0) {
                    if (pcesPermutation[i]!=pcesPermutation[j]){
                        eval(_sol);
                        bestFit = _sol.fitness();
                        tmp = _sol;
                        tmpPiece = tmp[pcesPermutation[i]];
                        tmp[pcesPermutation[i]] = tmp[pcesPermutation[j]];
                        tmp[pcesPermutation[j]] = tmpPiece;

                        for (unsigned int k=0; k<4; k++){
                            for (unsigned int m=0; m<4; m++){
                                tmp[pcesPermutation[i]].rotation = k;
                                tmp[pcesPermutation[j]].rotation = m;
                                eval(tmp);

                                if (bestFit > tmp.fitness()){
                                    bestRotationI = k;
                                    bestRotationJ = m;
                                    bestFit = tmp.fitness();
                                    //std::cout << "besFit " << bestFit << " RotationI " << bestRotationI << " RotationJ " << bestRotationJ << std::endl;
                                }
                            }
                        }
                        if (bestFit < _sol.fitness()){
                            tmp[pcesPermutation[i]].rotation = bestRotationI;
                            tmp[pcesPermutation[j]].rotation = bestRotationJ;
                            eval(tmp);
                            //std::cout << tmp.fitness() << std::endl;
                            _sol = tmp;
                            eval(_sol);
                            stratOne(_sol);
                            i = 256;
                            j = 256;
                        }

                    }
                }
            }
        }
    }

}

void localSearch::stratTwo(Solution & _sol) {

    unsigned int l = problem.taille.first;
    unsigned int h = problem.taille.second;
    double bestFit; //permet de verrifier si la fitness évolue
    int j = 0;

    evalSolution eval(problem); //???

    unsigned int bestRotation = 0;

    _sol.resize(l * h);

    for (unsigned int i = l + 1; i < h * l - 1 -l; i++) {

        //on enlève les pièces qui font parti du contour
        if (i % 16 != 0 && (i + 1) % 16 != 0) {
            eval(_sol);
            bestFit = _sol.fitness(); //on renseigne les paramètres initiaux avant modification
            bestRotation = _sol[i].rotation;
            j = 0;
            for(j=0;j<4;j++) {
                _sol[i].rotation = j ; //nouvelle rotation
                eval(_sol);
                if (bestFit < _sol.fitness()) { //retour à l'initial
                    _sol[i].rotation = bestRotation;
                } else if (bestFit > _sol.fitness()){ //nouveaux paramètres
                    bestRotation = _sol[i].rotation;
                    bestFit =_sol.fitness();
                }

            }
        }
    }
}