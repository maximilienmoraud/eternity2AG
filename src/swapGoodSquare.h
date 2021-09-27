#include "Solution.h"
#include "Problem.h"
#include "utils/eoRNG.h"
#include "eoOp.h"

int myrandomSQRT (int i) {
    return std::rand()%i;
}

class swapGoodSquare: public eoMonOp<Solution>
{
public:

    swapGoodSquare(Problem& _problem):problem(_problem){}

    bool operator()(Solution& _sol){

        unsigned int l=problem.taille.first;
        unsigned int h=problem.taille.second;

        _sol.resize(l*h);

        int carre1 = 0;
        int carre2 = 0;

        std::vector<unsigned int> pcesPermutation;

        srand(time(0));
        for(unsigned int i=0; i<(h)*(l); i++)
            pcesPermutation.push_back(i);
        std::random_shuffle(pcesPermutation.begin(),pcesPermutation.end(), myrandomSQRT);


        for(unsigned int i=0; i<l*h; i++) {
            if (pcesPermutation[i] > l - 1 && pcesPermutation[i] < l * h - 2*l && pcesPermutation[i] % l != 0 && (pcesPermutation[i] + 1) % l != 0) {
                if (_sol[pcesPermutation[i]].couleur[(3 - _sol[pcesPermutation[i]].rotation) % 4] ==
                    _sol[pcesPermutation[i] + 1].couleur[(5 - _sol[pcesPermutation[i] + 1].rotation) % 4] &&
                    _sol[pcesPermutation[i]].couleur[(4 - _sol[pcesPermutation[i]].rotation) % 4] ==
                    _sol[pcesPermutation[i] + l].couleur[(6 - _sol[pcesPermutation[i] + l].rotation) % 4] &&
                    _sol[pcesPermutation[i] + l].couleur[(3 - _sol[pcesPermutation[i] + l].rotation) % 4] ==
                    _sol[pcesPermutation[i] + l + 1].couleur[(5 - _sol[pcesPermutation[i] + l + 1].rotation) % 4] &&
                    _sol[pcesPermutation[i] + 1].couleur[(4 - _sol[pcesPermutation[i] + 1].rotation) % 4] ==
                    _sol[pcesPermutation[i] + l + 1].couleur[(6 - _sol[pcesPermutation[i] + l + 1].rotation) % 4]) {
                    carre1 = pcesPermutation[i];
                    break;
                }
            }
        }

        for(unsigned int i=0; i<l*h; i++) {
            if (pcesPermutation[i] > l - 1 && pcesPermutation[i] < l * h - 2*l && pcesPermutation[i] % l != 0 && (pcesPermutation[i] + 1) % l != 0) {
                if (pcesPermutation[i] != carre1 && pcesPermutation[i] != carre1+1 && pcesPermutation[i] != carre1-1 && pcesPermutation[i] != carre1-l && pcesPermutation[i] != carre1+1-l && pcesPermutation[i] != carre1-1-l && pcesPermutation[i] != carre1+l && pcesPermutation[i] != carre1+1+l && pcesPermutation[i] != carre1-1+l) {
                    if (_sol[pcesPermutation[i]].couleur[(3 - _sol[pcesPermutation[i]].rotation) % 4] ==
                        _sol[pcesPermutation[i] + 1].couleur[(5 - _sol[pcesPermutation[i] + 1].rotation) % 4] &&
                        _sol[pcesPermutation[i]].couleur[(4 - _sol[pcesPermutation[i]].rotation) % 4] ==
                        _sol[pcesPermutation[i] + l].couleur[(6 - _sol[pcesPermutation[i] + l].rotation) % 4] &&
                        _sol[pcesPermutation[i] + l].couleur[(3 - _sol[pcesPermutation[i] + l].rotation) % 4] ==
                        _sol[pcesPermutation[i] + l + 1].couleur[(5 - _sol[pcesPermutation[i] + l + 1].rotation) % 4] &&
                        _sol[pcesPermutation[i] + 1].couleur[(4 - _sol[pcesPermutation[i] + 1].rotation) % 4] ==
                        _sol[pcesPermutation[i] + l + 1].couleur[(6 - _sol[pcesPermutation[i] + l + 1].rotation) % 4]) {
                        carre2 = pcesPermutation[i];
                        break;
                    }
                }
            }
        }

        if (carre1 != 0 && carre2 != 0){
            //std::cout << carre1 << " " << carre2 << std::endl;
            Solution tmp;
            tmp.resize(l*h);

            tmp = _sol;
            tmp[carre1] = _sol[carre2];
            tmp[carre1+1] = _sol[carre2+1];
            tmp[carre1+l] = _sol[carre2+l];
            tmp[carre1+l+1] = _sol[carre2+l+1];

            tmp[carre2] = _sol[carre1];
            tmp[carre2+1] = _sol[carre1+1];
            tmp[carre2+l] = _sol[carre1+l];
            tmp[carre2+l+1] = _sol[carre1+l+1];

            _sol=tmp;

            _sol.invalidate();
            return true;
        } else{
            _sol.invalidate();
            return false;
        }
    }


private:
    Problem& problem;
};
