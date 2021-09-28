#include "Solution.h"
#include "initSolution.h"
#include "evalSolution.h"

initSolution::initSolution(Problem & _problem, unsigned int _strategie): problem(_problem), strategie(_strategie){}

void initSolution::setStrategie(unsigned int _strategie){
    strategie=_strategie;
}

void initSolution::operator()(Solution & _sol){
    if(strategie==0)
        initAleatoire(_sol);
    else if(strategie==1)
        initGlouton(_sol);
    else if(strategie==2)
        initGloutonBis(_sol);
    else if(strategie==3)
        initEscargot(_sol);
    else{
        std::cout << "Strategie non disponible" << std::endl;
    }

}

void initSolution::initAleatoire(Solution& _sol){
    //std::cout << "Strategie Aleatoire" << std::endl;
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    unsigned int b=0;
    unsigned int c=0;
    unsigned int r=0;

    std::vector<unsigned int> coinsPermutation;
    std::vector<unsigned int> bordsPermutation;
    std::vector<unsigned int> centresPermutation;

    for(unsigned int i=0; i<4; i++)
        coinsPermutation.push_back(i);
    std::random_shuffle(coinsPermutation.begin(),coinsPermutation.end());
//	for(unsigned int i=0; i<4; i++)
//		std::cout << coinsPermutation[i] << std::endl;

    for(unsigned int i=0; i<2*(l+h-4); i++)
        bordsPermutation.push_back(i);
    std::random_shuffle(bordsPermutation.begin(),bordsPermutation.end());
//	for(unsigned int i=0; i<2*(l+h-4); i++)
//		std::cout << bordsPermutation[i] << std::endl;

    for(unsigned int i=0; i<(h-2)*(l-2); i++)
        centresPermutation.push_back(i);
    std::random_shuffle(centresPermutation.begin(),centresPermutation.end());
//	for(unsigned int i=0; i<(h-2)*(l-2); i++)
//		std::cout << centresPermutation[i] << std::endl;

//	struct Piece p;
//	p.couleur[0]=1;
//	p.couleur[1]=1;
//	p.couleur[2]=1;
//	p.couleur[3]=1;
//	p.id=1;
//	p.nbBord=0;
//	p.rotation=1;

    _sol.resize(l*h);

    for(unsigned int i=0; i<_sol.size(); i++){
        //on affecte les coins
        if( (i==0) || (i==l-1) || (i==l*(h-1)) || (i== (l*h)-1) ){
            _sol[i]=problem.coins[coinsPermutation[c]];
            c++;
            if(i==0)
                _sol[i].rotation=1; //1
            else if(i==l-1)
                _sol[i].rotation=2; //2
            else if(i== (l*h)-1)
                _sol[i].rotation=3; //3
            else{
                _sol[i].rotation=0;
            }
        }
            //on affecte les bords
        else if( (i<l) || (i > l*(h-1)) || (i%l==0) || (i%l==l-1) ){
            _sol[i]=problem.bords[bordsPermutation[b]];
            b++;
            if(i<l)
                _sol[i].rotation=2; //2
            else if(i%l==0)
                _sol[i].rotation=1; //1
            else if(i%l==l-1)
                _sol[i].rotation=3;  //3
            else{
                _sol[i].rotation=0;
            }
        }
            //on affecte le reste
        else{
            _sol[i]=problem.centres[centresPermutation[r]];
            r++;
            _sol[i].rotation=floor(rng.uniform(4));
        }
        _sol[i].position=i;

    }

    //on replace les indices au bon endroit
    for(unsigned int i=0; i<_sol.size(); i++){
        struct Piece tmp = {};
        if(_sol[i].fixe){
            //std::cout << "changement en i " << i << std::endl;
            tmp=_sol[i];
            _sol[i]= _sol[problem.positionIndice(tmp.id)];
            _sol[problem.positionIndice(tmp.id)]=tmp;
            _sol[problem.positionIndice(tmp.id)].rotation=problem.rotationIndice(tmp.id);
        }
    }
}

//fonction random
int myrandomINIT (int i) {
    return std::rand()%i;
}

void initSolution::initGlouton(Solution& _sol){


    //std::cout << "Strategie Glouton" << std::endl;
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    unsigned int r=0;
    unsigned int lastColor=0;


    //on cree un mix de l'ordre de parcours des differentes pieces
    std::vector<unsigned int> coinsPermutation;
    std::vector<unsigned int> bordsPermutation;
    std::vector<unsigned int> centresPermutation;
    srand(time(0));
    for(unsigned int i=0; i<4; i++)
        coinsPermutation.push_back(i);
    std::random_shuffle(coinsPermutation.begin(),coinsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<4; i++)
    //std::cout << coinsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<2*(l+h-4); i++)
        bordsPermutation.push_back(i);
    std::random_shuffle(bordsPermutation.begin(),bordsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<bordsPermutation.size(); i++)
    //std::cout << bordsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<(h-2)*(l-2); i++)
        centresPermutation.push_back(i);
    std::random_shuffle(centresPermutation.begin(),centresPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<(h-2)*(l-2); i++)
    //std::cout << centresPermutation[i] << std::endl;


    _sol.resize(l*h);

    // on construit le contour
    for(unsigned int i=0; i<(2*l+2*(h-2)); i++){
        //on affecte le coin n°1
        if(i==0){
            _sol[i]=problem.coins[coinsPermutation[0]];
            _sol[i].rotation=1;
            _sol[i].position=i;
            coinsPermutation.erase(coinsPermutation.begin());
            lastColor = _sol[i].couleur[2];
        }
        //on affecte le bord n°1
        if(i > 0 && i<l-1){
            for (int b = 0; b < bordsPermutation.size(); ++b) {
                if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor){
                    //std::cout << problem.bords[bordsPermutation[b]].couleur[0] << " " << problem.bords[bordsPermutation[b]].couleur[1] << " " << problem.bords[bordsPermutation[b]].couleur[2] << " " << problem.bords[bordsPermutation[b]].couleur[3] << " "  << std::endl;
                    _sol[i]=problem.bords[bordsPermutation[b]];
                    _sol[i].rotation=2;
                    _sol[i].position=i;
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[i].couleur[1];
                    break;
                }
                if (b == bordsPermutation.size() - 1){
                    _sol[i]=problem.bords[bordsPermutation[b]];
                    _sol[i].rotation=2;
                    _sol[i].position=i;
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[i].couleur[1];
                }
            }

        }
        //on affecte le coin n°2
        if(i==l-1){
            for (int c = 0; c < coinsPermutation.size(); ++c) {
                if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor){
                    _sol[i]=problem.coins[coinsPermutation[c]];
                    _sol[i].rotation=2;
                    _sol[i].position=i;
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[i].couleur[2];
                    break;
                }
                if (c == coinsPermutation.size() - 1){
                    _sol[i]=problem.coins[coinsPermutation[c]];
                    _sol[i].rotation=2;
                    _sol[i].position=i;
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[i].couleur[2];
                }
            }
        }

        //on affecte le bord n°2
        if(i > l-1 && i < l+(h-2)){
            for (int b = 0; b < bordsPermutation.size(); ++b) {
                if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor){
                    _sol[(i-l+2)*l-1]=problem.bords[bordsPermutation[b]];
                    _sol[(i-l+2)*l-1].rotation=3;
                    _sol[(i-l+2)*l-1].position=(i-l+2)*l-1;
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(i-l+2)*l-1].couleur[1];
                    break;
                }
                if (b == bordsPermutation.size() - 1){
                    _sol[(i-l+2)*l-1]=problem.bords[bordsPermutation[b]];
                    _sol[(i-l+2)*l-1].rotation=3;
                    _sol[(i-l+2)*l-1].position=(i-l+2)*l-1;
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(i-l+2)*l-1].couleur[1];
                }
            }
        }

        //on affecte le coin n°3
        if(i == l+(h-2)){
            for (int c = 0; c < coinsPermutation.size(); ++c) {
                if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor){
                    _sol[(i-l+2)*l-1]=problem.coins[coinsPermutation[c]];
                    _sol[(i-l+2)*l-1].rotation=3;
                    _sol[(i-l+2)*l-1].position=(i-l+2)*l-1;
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[(i-l+2)*l-1].couleur[2];
                    break;
                }
                if (c == coinsPermutation.size() - 1){
                    _sol[(i-l+2)*l-1]=problem.coins[coinsPermutation[c]];
                    _sol[(i-l+2)*l-1].rotation=3;
                    _sol[(i-l+2)*l-1].position=(i-l+2)*l-1;
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[(i-l+2)*l-1].couleur[2];
                }
            }
        }

        //on affecte le bord n°3
        if(i > l+(h-2) && i < 2*l+(h-2)-1){
            for (int b = 0; b < bordsPermutation.size(); ++b) {
                if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor){
                    _sol[(l*h-1)-(i-(l+h-2))]=problem.bords[bordsPermutation[b]];
                    _sol[(l*h-1)-(i-(l+h-2))].rotation=0;
                    _sol[(l*h-1)-(i-(l+h-2))].position=(l*h-1)-(i-(l+h-2));
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(l*h-1)-(i-(l+h-2))].couleur[1];
                    break;
                }
                if (b == bordsPermutation.size() - 1){
                    _sol[(l*h-1)-(i-(l+h-2))]=problem.bords[bordsPermutation[b]];
                    _sol[(l*h-1)-(i-(l+h-2))].rotation=0;
                    _sol[(l*h-1)-(i-(l+h-2))].position=(l*h-1)-(i-(l+h-2));
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(l*h-1)-(i-(l+h-2))].couleur[1];
                }
            }
        }

        //on affecte le coin n°4
        if(i == l+(h-2)+l-1){
            for (int c = 0; c < coinsPermutation.size(); ++c) {
                if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor){
                    _sol[(l*h-1)-(i-(l+h-2))]=problem.coins[coinsPermutation[c]];
                    _sol[(l*h-1)-(i-(l+h-2))].rotation=0;
                    _sol[(l*h-1)-(i-(l+h-2))].position=(l*h-1)-(i-(l+h-2));
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[(l*h-1)-(i-(l+h-2))].couleur[2];
                    break;
                }
                if (c == coinsPermutation.size() - 1){
                    _sol[(l*h-1)-(i-(l+h-2))]=problem.coins[coinsPermutation[c]];
                    _sol[(l*h-1)-(i-(l+h-2))].rotation=0;
                    _sol[(l*h-1)-(i-(l+h-2))].position=(l*h-1)-(i-(l+h-2));
                    coinsPermutation.erase(coinsPermutation.begin()+c);
                    lastColor = _sol[(l*h-1)-(i-(l+h-2))].couleur[2];
                }
            }
        }
        //on affecte le bord n°4
        if(i > l+(h-2)+l-1){
            for (int b = 0; b < bordsPermutation.size(); ++b) {
                if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor){
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)]=problem.bords[bordsPermutation[b]];
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)].rotation=1;
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)].position=(l*h-l)-((i-(2*l+h-3))*l);
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(l*h-l)-((i-(2*l+h-3))*l)].couleur[1];
                    break;
                }
                if (b == bordsPermutation.size() - 1){
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)]=problem.bords[bordsPermutation[b]];
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)].rotation=1;
                    _sol[(l*h-l)-((i-(2*l+h-3))*l)].position=(l*h-l)-((i-(2*l+h-3))*l);
                    bordsPermutation.erase(bordsPermutation.begin()+b);
                    lastColor = _sol[(l*h-l)-((i-(2*l+h-3))*l)].couleur[1];
                }
            }
        }
        //std::cout << lastColor << std::endl;
    }

    //on parcours le puzzle
    for(unsigned int i=0; i<l*h; i++){
        //on garde que les centres
        if (i > l-1 && i < l*h-l && i%16!=0 && (i+1)%16!=0){

            unsigned int correspondingSide = 0;
            unsigned int rotationNeeded = 0;
            unsigned int indexBestPiece = 0;

            // pour chaque piece de centre pas encore placé
            for (int c = 0; c < centresPermutation.size(); ++c) {
                //on cherche si une solution est "optimale" en fonction de toutes les rotations possibles
                for (int j = 0; j < 4; ++j) {
                    if (_sol[i-1].couleur[(3-_sol[i-1].rotation)%4]!=0 && _sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                        if (correspondingSide < 1){
                            correspondingSide = 1;
                            if (1-j == -1){
                                rotationNeeded = 3;
                            }else if (1-j == -2){
                                rotationNeeded = 2;
                            }else{
                                rotationNeeded = 1-j;
                            }
                            indexBestPiece = c;
                        }
                        if (j==4){
                            j=0;
                        }
                        else {
                            ++j;
                        }
                        if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]!=0 && _sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide < 2){
                                correspondingSide = 2;
                                if (2-j == -1){
                                    rotationNeeded = 3;
                                }else{
                                    rotationNeeded = 2-j;
                                }
                                indexBestPiece = c;
                            }
                            if (j==4){
                                j=0;
                            }
                            else {
                                ++j;
                            }
                            if (_sol[i+1].couleur[(5-_sol[i+1].rotation)%4]!=0 && _sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                                correspondingSide = 3;
                                rotationNeeded = 3-j;
                                indexBestPiece = c;
                                break;
                            }
                        }
                    }
                }
            }
            //on sauvegarde la meilleure solution
            _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
            _sol[i].rotation=rotationNeeded;
            _sol[i].position=i;
            centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
        }
    }
}

//fonction de backtracking construction du contour
void BTtour(Solution& _sol, std::vector<unsigned int> coinsPermutation, std::vector<unsigned int> bordsPermutation, Problem problem, unsigned int lastColor=0, unsigned int i = 0){
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;

    //std::cout << i << std::endl;

    if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
        return;
    }
    //on affecte le coin n°1
    if (i == 0) {
        _sol[i] = problem.coins[coinsPermutation[0]];
        _sol[i].rotation = 1;
        _sol[i].position = i;
        coinsPermutation.erase(coinsPermutation.begin());
        //std::cout << i << std::endl;
        BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[i].couleur[2], i + 1);
        return;
    }
    //on affecte le bord n°1
    if (i > 0 && i < l - 1) {
        //std::cout << "i : " << i << std::endl;
        for (int b = 0; b < bordsPermutation.size(); ++b) {
            //std::cout << "i : " << i << "  b : " << b << std::endl;
            if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor) {
                //std::cout << problem.bords[bordsPermutation[b]].couleur[0] << " " << problem.bords[bordsPermutation[b]].couleur[1] << " " << problem.bords[bordsPermutation[b]].couleur[2] << " " << problem.bords[bordsPermutation[b]].couleur[3] << " "  << std::endl;
                unsigned int workingPiece = bordsPermutation[b];
                _sol[i] = problem.bords[bordsPermutation[b]];
                _sol[i].rotation = 2;
                _sol[i].position = i;
                bordsPermutation.erase(bordsPermutation.begin() + b);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[i].couleur[1], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                bordsPermutation.push_back(workingPiece);
            }
        }return;
    }
    //on affecte le coin n°2
    if (i == l - 1) {
        for (int c = 0; c < coinsPermutation.size(); ++c) {
            if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor) {
                unsigned int workingPiece = coinsPermutation[c];
                _sol[i] = problem.coins[coinsPermutation[c]];
                _sol[i].rotation = 2;
                _sol[i].position = i;
                coinsPermutation.erase(coinsPermutation.begin() + c);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[i].couleur[2], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                coinsPermutation.push_back(workingPiece);
            }
        }return;
    }

    //on affecte le bord n°2
    if (i > l - 1 && i < l + (h - 2)) {
        for (int b = 0; b < bordsPermutation.size(); ++b) {
            if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor) {
                unsigned int workingPiece = bordsPermutation[b];
                _sol[(i - l + 2) * l - 1] = problem.bords[bordsPermutation[b]];
                _sol[(i - l + 2) * l - 1].rotation = 3;
                _sol[(i - l + 2) * l - 1].position = (i - l + 2) * l - 1;
                bordsPermutation.erase(bordsPermutation.begin() + b);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[(i - l + 2) * l - 1].couleur[1], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                bordsPermutation.push_back(workingPiece);
            }
        }return;
    }

    //on affecte le coin n°3
    if (i == l + (h - 2)) {
        for (int c = 0; c < coinsPermutation.size(); ++c) {
            if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor) {
                unsigned int workingPiece = coinsPermutation[c];
                _sol[(i - l + 2) * l - 1] = problem.coins[coinsPermutation[c]];
                _sol[(i - l + 2) * l - 1].rotation = 3;
                _sol[(i - l + 2) * l - 1].position = (i - l + 2) * l - 1;
                coinsPermutation.erase(coinsPermutation.begin() + c);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[(i - l + 2) * l - 1].couleur[2], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                coinsPermutation.push_back(workingPiece);
            }
        }return;
    }

    //on affecte le bord n°3
    if (i > l + (h - 2) && i < 2 * l + (h - 2) - 1) {
        for (int b = 0; b < bordsPermutation.size(); ++b) {
            if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor) {
                unsigned int workingPiece = bordsPermutation[b];
                _sol[(l * h - 1) - (i - (l + h - 2))] = problem.bords[bordsPermutation[b]];
                _sol[(l * h - 1) - (i - (l + h - 2))].rotation = 0;
                _sol[(l * h - 1) - (i - (l + h - 2))].position = (l * h - 1) - (i - (l + h - 2));
                bordsPermutation.erase(bordsPermutation.begin() + b);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[(l * h - 1) - (i - (l + h - 2))].couleur[1], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                bordsPermutation.push_back(workingPiece);
            }
        }return;
    }

    //on affecte le coin n°4
    if (i == l + (h - 2) + l - 1) {
        for (int c = 0; c < coinsPermutation.size(); ++c) {
            if (problem.coins[coinsPermutation[c]].couleur[3] == lastColor) {
                unsigned int workingPiece = coinsPermutation[c];
                _sol[(l * h - 1) - (i - (l + h - 2))] = problem.coins[coinsPermutation[c]];
                _sol[(l * h - 1) - (i - (l + h - 2))].rotation = 0;
                _sol[(l * h - 1) - (i - (l + h - 2))].position = (l * h - 1) - (i - (l + h - 2));
                coinsPermutation.erase(coinsPermutation.begin() + c);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[(l * h - 1) - (i - (l + h - 2))].couleur[2], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                coinsPermutation.push_back(workingPiece);
            }
        }return;
    }
    //on affecte le bord n°4
    if (i > l + (h - 2) + l - 1) {
        for (int b = 0; b < bordsPermutation.size(); ++b) {
            if (problem.bords[bordsPermutation[b]].couleur[3] == lastColor) {
                unsigned int workingPiece = bordsPermutation[b];
                _sol[(l * h - l) - ((i - (2 * l + h - 3)) * l)] = problem.bords[bordsPermutation[b]];
                _sol[(l * h - l) - ((i - (2 * l + h - 3)) * l)].rotation = 1;
                _sol[(l * h - l) - ((i - (2 * l + h - 3)) * l)].position = (l * h - l) - ((i - (2 * l + h - 3)) * l);
                bordsPermutation.erase(bordsPermutation.begin() + b);
                BTtour(_sol, coinsPermutation, bordsPermutation, problem, _sol[(l * h - l) - ((i - (2 * l + h - 3)) * l)].couleur[1], i + 1);
                if (_sol[l].couleur[1] == _sol[0].couleur[3] && _sol[0].id!=0){
                    return;
                }
                bordsPermutation.push_back(workingPiece);
            }
        }return;
    }
}

// meme algo que le precedent sauf que le contour est generé avec le back tracking
void initSolution::initGloutonBis(Solution& _sol){
    //std::cout << "Strategie Glouton" << std::endl;
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    evalSolution eval(problem);

    std::vector<unsigned int> coinsPermutation;
    std::vector<unsigned int> bordsPermutation;
    std::vector<unsigned int> centresPermutation;

    srand(time(0));
    for(unsigned int i=0; i<4; i++)
        coinsPermutation.push_back(i);
    std::random_shuffle(coinsPermutation.begin(),coinsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<4; i++)
    //std::cout << coinsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<2*(l+h-4); i++)
        bordsPermutation.push_back(i);
    std::random_shuffle(bordsPermutation.begin(),bordsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<bordsPermutation.size(); i++)
    //    std::cout << bordsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<(h-2)*(l-2); i++)
        centresPermutation.push_back(i);
    std::random_shuffle(centresPermutation.begin(),centresPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<(h-2)*(l-2); i++)
    //std::cout << centresPermutation[i] << std::endl;

    _sol.resize(l * h);

    BTtour(_sol, coinsPermutation, bordsPermutation, problem);

    /*
    //test id all different
    std::vector<unsigned int> testID;
    for(unsigned int i=0; i<l*h; i++) {
        if (i > l - 1 && i < l * h - l && i % l != 0 && (i + 1) % l != 0) {
        }else{
            std::cout << _sol[i].id << std::endl;
            if (std::find(testID.begin(), testID.end(), _sol[i].id) != testID.end()){
                std::cout << "erreur duplication" << std::endl;
                break;
            }
            testID.push_back(_sol[i].id);
        }}
    */

    //Parcourir l'ensemble des positions du tableau
    for(unsigned int i=0; i<l*h; i++){

        //on garde que ls positions valables pour les centres
        if (i > l-1 && i < l*h-l && i%l!=0 && (i+1)%l!=0){ //RAZ position des centres uniquement pour un 16*16?
            unsigned int correspondingSide = 0;
            unsigned int rotationNeeded = 0;
            unsigned int indexBestPiece = 0;

            //std::cout << _sol[i-1].couleur[abs(3-_sol[i-1].rotation)] << " " << _sol[i-l].couleur[abs(0-_sol[i-l].rotation)] << " " << _sol[i+1].couleur[abs(1-_sol[i+1].rotation)] << " " << _sol[i+l].couleur[abs(2-_sol[i-l].rotation)] << " "  << std::endl;
            for (int c = 0; c < centresPermutation.size(); ++c) { //parcours des indices des centres

                for (int j = 0; j < 4; ++j) {
                    if (_sol[i-1].couleur[(3-_sol[i-1].rotation)%4]!=0 && _sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                        if (correspondingSide < 1){
                            correspondingSide = 1;
                            if (1-j == -1){
                                rotationNeeded = 3;
                            }else if (1-j == -2){
                                rotationNeeded = 2;
                            }else{
                                rotationNeeded = 1-j;
                            }
                            indexBestPiece = c;
                        }
                        if (j==4){
                            j=0;
                        }
                        else {
                            ++j;
                        }
                        if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]!=0 && _sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide < 2){
                                correspondingSide = 2;
                                if (2-j == -1){
                                    rotationNeeded = 3;
                                }else{
                                    rotationNeeded = 2-j;
                                }
                                indexBestPiece = c;
                            }
                            if (j==4){
                                j=0;
                            }
                            else {
                                ++j;
                            }
                            if (_sol[i+1].couleur[(5-_sol[i+1].rotation)%4]!=0 && _sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                                correspondingSide = 3;
                                rotationNeeded = 3-j;
                                indexBestPiece = c;
                                break;
                            }
                        }
                    }
                }
            }
            _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
            _sol[i].rotation=rotationNeeded;
            _sol[i].position=i;
            centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
        }
    }

}


void initSolution::initEscargot(Solution& _sol){

    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    evalSolution eval(problem);

    std::vector<unsigned int> coinsPermutation;
    std::vector<unsigned int> bordsPermutation;
    std::vector<unsigned int> centresPermutation;

    srand(time(0));
    for(unsigned int i=0; i<4; i++)
        coinsPermutation.push_back(i);
    std::random_shuffle(coinsPermutation.begin(),coinsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<4; i++)
    //std::cout << coinsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<2*(l+h-4); i++)
        bordsPermutation.push_back(i);
    std::random_shuffle(bordsPermutation.begin(),bordsPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<bordsPermutation.size(); i++)
    //    std::cout << bordsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<(h-2)*(l-2); i++)
        centresPermutation.push_back(i);
    std::random_shuffle(centresPermutation.begin(),centresPermutation.end(),myrandomINIT);
    //for(unsigned int i=0; i<(h-2)*(l-2); i++)
    //std::cout << centresPermutation[i] << std::endl;

    _sol.resize(l * h);

    BTtour(_sol, coinsPermutation, bordsPermutation, problem);

    //Remplir les centre en suivant la méthode de l'escargot
    int x = l; // permet de connaitre l'emplacement de la dernière pièce placée sur le puzzle
    int increm = 1; //permet de géréer l'incrémentation des calculs
    int total_p = h*l-2*(h+l-2); //permet de connaitre le nombre de pièce qu'il reste à placer
    int cas = 1; //permet de connaitre quel coté on est entrain de remplir : ligne du haut : 1, colonne de droite : 2, ligne du bas : 3, colonne de gauche : 4

    while(total_p >0 ){

        if (cas==1){
            for(int i=x+1; i <= (l-1)*(1+increm) ; i++){ //Parcours de la ligne du haut
                unsigned int correspondingSide = 0;
                unsigned int rotationNeeded = 0;
                unsigned int indexBestPiece = 0;

                for (int c = 0; c < centresPermutation.size(); ++c) { //parcours des indices des centres
                    for (int j = 0; j < 4; ++j) { //parcours de toutes les couleurs d'une pièce

                        //On verrifie de gauche
                        if (_sol[i-1].couleur[(3-_sol[i-1].rotation)%4]!=0 && _sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide < 1){
                                correspondingSide = 1;
                                if (1-j == -1){ //j=2
                                    rotationNeeded = 3;
                                }else if (1-j == -2){ //j=3
                                    rotationNeeded = 2;
                                }else{ //j = 0 ou 1
                                    rotationNeeded = 1-j;  //1 ou 0
                                }
                                indexBestPiece = c;
                            }
                            ++j; // +1 à j

                            //comparaison à la pièce du dessus
                            if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]!=0 && _sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                if (correspondingSide < 2){
                                    correspondingSide = 2;
                                    if (j == 3){ //j=3
                                        rotationNeeded = 3;
                                    } else if(j==4){
                                        rotationNeeded=2;
                                    }else{
                                        rotationNeeded = 2-j;
                                    }
                                    indexBestPiece = c;
                                }
                                if(_sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==0){
                                    break;
                                }
                                if (j==4){
                                    j=0;
                                }
                                else {
                                    ++j;
                                }

                                //on regarde la pièce à la droite
                                if ( _sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                    correspondingSide = 3;
                                    rotationNeeded = 3-(j%4);
                                    indexBestPiece = c;
                                    break;
                                }
                                if (j==0){
                                    j=4;
                                }
                            }
                        }
                    }
                }
                _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
                _sol[i].rotation=rotationNeeded;
                _sol[i].position=i;
                centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
                x=i; //changer la valeur de x
                total_p --; // ajout d'une pièce donc décrémenter le total
            }
            cas=2;
        }
        else if (cas==2){
            for(int i=x+l; i<= (l*h-1)-increm*(1+l) ; i=i+l){ //parcours colonne de droite
                unsigned int correspondingSide = 0;
                unsigned int rotationNeeded = 0;
                unsigned int indexBestPiece = 0;

                for (int c = 0; c < centresPermutation.size(); ++c) { //parcours des indices des centres

                    for (int j = 0; j < 4; ++j) { //parcours de toutes les couleurs d'une pièce

                        //On verrifie la pièce du dessus
                        if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]!=0 && _sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide < 1) {
                                correspondingSide = 1;
                                if (2 - j == -1) { //j=3
                                    rotationNeeded = 3;
                                } else {
                                    rotationNeeded = 2 - j;
                                }
                                indexBestPiece = c;
                            }
                            ++j; // +1 à j
                            //comparaison à la pièce de droite
                            if (_sol[i+1].couleur[(5-_sol[i+1].rotation)%4]!=0 && _sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                if (correspondingSide<2){
                                    correspondingSide=2;
                                    rotationNeeded = 3-(j%4);
                                    indexBestPiece = c;
                                }
                                if(_sol[i+l].couleur[(6-_sol[i+l].rotation)%4]==0){
                                    break;
                                }
                                if (j==4){
                                    j=0;
                                }
                                else {
                                    ++j;
                                }
                                //on regarde la pièce du bas
                                if (_sol[i+l].couleur[(6-_sol[i+l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                    correspondingSide = 3;
                                    if (j == 0 ) { //j=0
                                        rotationNeeded = 0;
                                    } else { //j = 1 ou 2 ou 3
                                        rotationNeeded = 4 - j;  //2 ou 3 ou 1 ou 4 (4-4 =0 revient bien au cas j=0)
                                    }
                                    indexBestPiece = c;
                                    break;
                                }
                                if (j==0){
                                    j=4;
                                }
                            }
                        }
                    }

                }
                _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
                _sol[i].rotation=rotationNeeded;
                _sol[i].position=i;
                centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
                x=i;
                total_p --;
            }
            cas=3;
        }
        else if (cas==3){
            for(int i=x-1; i >= (l*h)-l-increm*(l-1) ; i--){ //parcours ligne du bas
                unsigned int correspondingSide = 0;
                unsigned int rotationNeeded = 0;
                unsigned int indexBestPiece = 0;

                for (int c = 0; c < centresPermutation.size(); ++c) { //parcours des indices des centres

                    for (int j = 0; j < 4; ++j) { //parcours de toutes les couleurs d'une pièce

                        //On verrifie la pièce de droite
                        if (_sol[i+1].couleur[(5-_sol[i+1].rotation)%4]!=0 && _sol[i+1].couleur[(5-_sol[i+1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide<1) {
                                correspondingSide = 1;
                                rotationNeeded = 3 - j;
                                indexBestPiece = c;
                            }
                            ++j; // +1 à j

                            //comparaison à la pièce de bas
                            if (_sol[i+l].couleur[(6-_sol[i+l].rotation)%4]!=0 && _sol[i+l].couleur[(6-_sol[i+l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                if(correspondingSide<2){
                                    correspondingSide = 2;
                                    if (j == 0) { //j=0
                                        rotationNeeded = 0;
                                    } else { //j = 1 ou 2 ou 3
                                        rotationNeeded = 4 - j;  //2 ou 3 ou 1
                                    }
                                    indexBestPiece = c;
                                }
                                if(_sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==0){
                                    break;
                                }
                                if (j==4){
                                    j=0;
                                }
                                else {
                                    ++j;
                                }

                                //on regarde la pièce de gauche
                                if ( _sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                    correspondingSide = 3;

                                    if (j == 2){ //j=2
                                        rotationNeeded = 3;
                                    }else if (j == 3){ //j=3
                                        rotationNeeded = 2;
                                    }else{ //j = 0 ou 1
                                        rotationNeeded = 1-(j%4);  //1 ou 0
                                    }
                                    indexBestPiece = c;
                                    break;

                                }
                                if (j==0){
                                    j=4;
                                }
                            }
                        }
                    }

                }
                _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
                _sol[i].rotation=rotationNeeded;
                _sol[i].position=i;
                centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
                x=i;
                total_p --;
            }
            cas=4;
        }
        else {
            for(int i=x-l; i >= l+increm*(l+1) ; i=i-l){ //parcours colonne de gauche
                unsigned int correspondingSide = 0;
                unsigned int rotationNeeded = 0;
                unsigned int indexBestPiece = 0;

                for (int c = 0; c < centresPermutation.size(); ++c) { //parcours des indices des centres

                    for (int j = 0; j < 4; ++j) { //parcours de toutes les couleurs d'une pièce

                        //On verrifie la pièce du bas
                        if (_sol[i+l].couleur[(6-_sol[i+l].rotation)%4]!=0 && _sol[i+l].couleur[(6-_sol[i+l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j]){
                            if(correspondingSide<1){
                                correspondingSide = 1;
                                if (j == 0) { //j=0
                                    rotationNeeded = 0;
                                } else { //j = 1 ou 2 ou 3
                                    rotationNeeded = 4 - j;  //2 ou 3 ou 1
                                }
                                indexBestPiece = c;
                            }
                            if(j==4){// +1 à j
                                j=0;
                            } else{
                                j++;
                            }
                            //comparaison à la pièce de gauche
                            if (_sol[i-1].couleur[(3-_sol[i-1].rotation)%4]!=0 && _sol[i-1].couleur[(3-_sol[i-1].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                if (correspondingSide <2){
                                    correspondingSide = 2;
                                    if (j == 2){ //j=2
                                        rotationNeeded = 3;
                                    }else if (j == 3){ //j=3
                                        rotationNeeded = 2;
                                    }else{ //j = 0 ou 1
                                        rotationNeeded = 1-(j%4);  //1 ou 0
                                    }
                                    indexBestPiece = c;
                                }
                                if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==0){
                                    break;
                                }
                                if (j==4){
                                    j=0;
                                }
                                else {
                                    ++j;
                                }

                                //on regarde la pièce du haut
                                if (_sol[i-l].couleur[(4-_sol[i-l].rotation)%4]==problem.centres[centresPermutation[c]].couleur[j%4]){
                                    correspondingSide = 3;
                                    if (j == 3){ //j=3
                                        rotationNeeded = 3;
                                    }else{
                                        rotationNeeded = 2-(j%4);
                                    }
                                    indexBestPiece = c;
                                    break;

                                }
                                if (j==0){
                                    j=4;
                                }
                            }
                        }
                    }

                }
                _sol[i]=problem.centres[centresPermutation[indexBestPiece]];
                _sol[i].rotation=rotationNeeded;
                _sol[i].position=i;
                centresPermutation.erase(centresPermutation.begin()+indexBestPiece);
                x=i;
                total_p --;
            }
            cas=1;
            increm ++;
        }

    } //fin du while

    std::vector<unsigned int> testID;
    for(unsigned int i=0; i<l*h; i++) {
        if (std::find(testID.begin(), testID.end(), _sol[i].id) != testID.end()){
            std::cout << "erreur duplication" << std::endl;
            break;
        }
        testID.push_back(_sol[i].id);
    }
}