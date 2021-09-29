#include "localSearch.h"
#include "Solution.h"
#include "Piece.h"
#include "evalSolution.h"
#include <eo>


localSearch::localSearch(Problem & _problem, unsigned int _strategie): problem(_problem), strategie(_strategie){}

//fonction random
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

//Méthode de descente avec première amélioration
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

    //on mix l'ordre de parcours des pieces
    srand(time(0));
    for(unsigned int i=0; i<(h)*(l); i++)
        pcesPermutation.push_back(i);
    std::random_shuffle(pcesPermutation.begin(),pcesPermutation.end(), myrandomLS);

    // on parcours le puzzle
    for(unsigned int i=0; i<l*h; i++) {
        //std::cout << "i : " << i << std::endl;
        //on fait en sorte de ne pas etre sur le contour
        if (pcesPermutation[i] > l - 1 && pcesPermutation[i] < l * h - l && pcesPermutation[i] % l != 0 && (pcesPermutation[i] + 1) % l != 0) {
            // deuxieme boucle de parcours du puzzle
            for(unsigned int j=i; j<l*h; j++) {
                //on fait en sorte de ne pas etre sur le contour
                if (pcesPermutation[j] > l - 1 && pcesPermutation[j] < l * h - l && pcesPermutation[j] % l != 0 && (pcesPermutation[j] + 1) % l != 0) {
                    //on regarde si une permutation peut être benifique
                    if (pcesPermutation[i]!=pcesPermutation[j]){
                        eval(_sol);
                        bestFit = _sol.fitness();
                        tmp = _sol;
                        tmpPiece = tmp[pcesPermutation[i]];
                        tmp[pcesPermutation[i]] = tmp[pcesPermutation[j]];
                        tmp[pcesPermutation[j]] = tmpPiece;
                        //on teste ttoutes les rotations possible pour chaque echange
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
                        // si fitness amélioré on applique le changement + appel recursif
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

// Recherche de la meilleure rotation pour chacune des pièces du puzzle
void localSearch::stratTwo(Solution & _sol) {

    unsigned int l = problem.taille.first;
    unsigned int h = problem.taille.second;
    double bestFit; //permet de verrifier si la fitness évolue
    int j = 0;

    evalSolution eval(problem);
    unsigned int bestRotation = 0;
    _sol.resize(l * h);

    //Parcours de toutes les pièces du centre du puzzle
    for (unsigned int i = l + 1; i < h * l - 1 -l; i++) {
        if (i % l != 0 && (i + 1) % l != 0) { //on enlève les pièces qui sont sur les extrémintés

            //on initialise les valeurs par rapport à la position initiale de la pièce
            eval(_sol);
            bestFit = _sol.fitness(); //on renseigne les paramètres initiaux avant modification
            bestRotation = _sol[i].rotation;
            j = 0;

            // Test pour savoir si une meilleure rotation existe
            for(j=0;j<4;j++) {    //on test toutes les rotations possibles
                _sol[i].rotation = j ; //nouvelle rotation
                eval(_sol);
                if (bestFit < _sol.fitness()) { //retour à l'initial parce que le score est moins bon
                    _sol[i].rotation = bestRotation;
                } else if (bestFit > _sol.fitness()){ //nouveaux paramètres car le score est meilleur
                    bestRotation = _sol[i].rotation;
                    bestFit =_sol.fitness();
                }

            }
        }
    }
}