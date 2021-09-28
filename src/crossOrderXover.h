#include "Solution.h"
#include "Problem.h"
#include "utils/eoRNG.h"
#include "eoOp.h"

class crossOrderXover: public eoQuadOp<Solution>
{
public:

    crossOrderXover(Problem& _problem, int _nbLigne):problem(_problem),nbLigne(_nbLigne){}

    bool operator()(Solution& _sol1, Solution& _sol2){
        unsigned int l=problem.taille.first;
        unsigned int h=problem.taille.second;

        bool check = true;

        Solution tmp1;
        Solution tmp2;

        tmp1.resize(l*h);
        tmp2.resize(l*h);

        _sol1.resize(l*h);
        _sol2.resize(l*h);

        std::vector<unsigned int> lignesPermutation1;
        std::vector<unsigned int> lignesPermutation2;
        std::vector<double> indexList1;
        std::vector<double> indexList2;

        // on prend n ligne aléatoirment mais elles doivent être differentres x2
        for(unsigned int i=0; i<nbLigne; i++){
            int randomNB;

            do{
                check = false;
                randomNB = rand() % (h-2) + 1;
                for (int j = 0; j < i; ++j) {
                    if (randomNB==lignesPermutation1[j]){
                        check = true;
                    }
                }
            }while (check);
            lignesPermutation1.push_back(randomNB);

            do{
                check = false;
                randomNB = rand() % (h-2) + 1;
                for (int j = 0; j < i; ++j) {
                    if (randomNB==lignesPermutation2[j]){
                        check = true;
                    }
                }
            }while (check);
            lignesPermutation2.push_back(randomNB);


            //std::cout << "Lignes selectionnées : " << lignesPermutation1[i] << std::endl;
            //std::cout << "Lignes selectionnées Arrvié : " << lignesPermutation2[i] << std::endl;

            // on permute les lignes dans des puzzles temp vies
            for (int j = 1; j < l-1; ++j) {
                //std::cout << "INDEXs selectionnés : " << j << "  IDs correspondants : " << _sol2[j].id << std::endl;
                tmp1[lignesPermutation1[i]*l+j] = _sol2[lignesPermutation2[i]*l+j];
                indexList1.push_back(_sol2[lignesPermutation2[i]*l+j].id);
                tmp2[lignesPermutation2[i]*l+j] = _sol1[lignesPermutation1[i]*l+j];
                indexList2.push_back(_sol1[lignesPermutation1[i]*l+j].id);
            }
        }

        //on rerempli en verifiant qu'il n'y ait pas de doublon
        for(unsigned int i=0; i<l*h; i++) {
            if (i > l - 1 && i < l * h - l && i % l != 0 && (i + 1) % l != 0) {

            }else{
                tmp1[i] = _sol1[i];
                indexList1.push_back(_sol1[i].id);
                tmp2[i] = _sol2[i];
                indexList2.push_back(_sol2[i].id);
            }}

        int offset1 = l+1;
        int offset2 = l+1;
        for(unsigned int i=0; i<l*h; i++) {
            if (i > l - 1 && i < l * h - l && i % l != 0 && (i + 1) % l != 0) {
                if (tmp1[i].id==0){
                    while (std::find(indexList1.begin(), indexList1.end(), _sol1[offset1].id) != indexList1.end()){
                        offset1 ++;
                    }
                    tmp1[i] = _sol1[offset1];
                    offset1 ++;
                }
                if (tmp2[i].id==0){
                    while (std::find(indexList2.begin(), indexList2.end(), _sol2[offset2].id) != indexList2.end()){
                        offset2 ++;
                    }
                    tmp2[i] = _sol2[offset2];
                    offset2 ++;
                }
            }
        }

        _sol1 = tmp1;
        _sol2 = tmp2;



        _sol1.invalidate();
        _sol2.invalidate();
        return true;
    }

private:
    Problem& problem;
    int nbLigne;
};