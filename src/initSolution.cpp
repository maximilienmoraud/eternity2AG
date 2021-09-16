#include "Solution.h" 
#include "initSolution.h"
#include <time.h>
#include <stdlib.h>

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

void initSolution::initGlouton(Solution& _sol){
	//std::cout << "Strategie Glouton" << std::endl;
	unsigned int l=problem.taille.first;
	unsigned int h=problem.taille.second;
	unsigned int r=0;
	unsigned int lastColor=0;

	std::vector<unsigned int> coinsPermutation;
	std::vector<unsigned int> bordsPermutation;
	std::vector<unsigned int> centresPermutation;

	

	for(unsigned int i=0; i<4; i++)
		coinsPermutation.push_back(i);
	std::random_shuffle(std::begin(coinsPermutation),std::end(coinsPermutation));
	//for(unsigned int i=0; i<4; i++)
		//std::cout << coinsPermutation[i] << std::endl;

	for(unsigned int i=0; i<2*(l+h-4); i++)
		bordsPermutation.push_back(i);
	std::random_shuffle(std::begin(bordsPermutation),std::end(bordsPermutation));
	//for(unsigned int i=0; i<bordsPermutation.size(); i++)
		//std::cout << bordsPermutation[i] << std::endl;

	for(unsigned int i=0; i<(h-2)*(l-2); i++)
		centresPermutation.push_back(i);
	std::random_shuffle(std::begin(centresPermutation),std::end(centresPermutation));
	//for(unsigned int i=0; i<(h-2)*(l-2); i++)
		//std::cout << centresPermutation[i] << std::endl;


	_sol.resize(l*h);

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

	for(unsigned int i=0; i<l*h; i++){
	    //on garde que les centres
	    if (i > l-1 && i < l*h-l && i%16!=0 && (i+1)%16!=0){
	        unsigned int correspondingSide = 0;
	        unsigned int rotationNeeded = 0;
	        unsigned int indexBestPiece = 0;
	        //std::cout << _sol[i-1].couleur[abs(3-_sol[i-1].rotation)] << " " << _sol[i-l].couleur[abs(0-_sol[i-l].rotation)] << " " << _sol[i+1].couleur[abs(1-_sol[i+1].rotation)] << " " << _sol[i+l].couleur[abs(2-_sol[i-l].rotation)] << " "  << std::endl;
	        for (int c = 0; c < centresPermutation.size(); ++c) {


                for (int j = 0; j < 4; ++j) {
                    if (_sol[i-1].couleur[abs(3-_sol[i-1].rotation)]!=0 && _sol[i-1].couleur[abs(3-_sol[i-1].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
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
                        if (_sol[i-l].couleur[abs(0-_sol[i-l].rotation)]!=0 && _sol[i-l].couleur[abs(0-_sol[i-l].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
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
                            if (_sol[i+1].couleur[abs(1-_sol[i+1].rotation)]!=0 && _sol[i+1].couleur[abs(1-_sol[i+1].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
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

	//on replace les indices au bon endroit
	/*
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
*/
}

void initSolution::initGloutonBis(Solution& _sol){
    //std::cout << "Strategie Glouton" << std::endl;
    unsigned int l=problem.taille.first;
    unsigned int h=problem.taille.second;
    unsigned int r=0;
    unsigned int lastColor=0;

    std::vector<unsigned int> coinsPermutation;
    std::vector<unsigned int> bordsPermutation;
    std::vector<unsigned int> centresPermutation;


    srand(time(0));
    for(unsigned int i=0; i<4; i++)
        coinsPermutation.push_back(i);
    std::random_shuffle(std::begin(coinsPermutation),std::end(coinsPermutation));
    //for(unsigned int i=0; i<4; i++)
    //std::cout << coinsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<2*(l+h-4); i++)
        bordsPermutation.push_back(i);
    std::random_shuffle(std::begin(bordsPermutation),std::end(bordsPermutation));
    //for(unsigned int i=0; i<bordsPermutation.size(); i++)
    //std::cout << bordsPermutation[i] << std::endl;
    srand(time(0));
    for(unsigned int i=0; i<(h-2)*(l-2); i++)
        centresPermutation.push_back(i);
    std::random_shuffle(std::begin(centresPermutation),std::end(centresPermutation));
    //for(unsigned int i=0; i<(h-2)*(l-2); i++)
    //std::cout << centresPermutation[i] << std::endl;


    _sol.resize(l*h);

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

    for(unsigned int i=0; i<l*h; i++){
        //on garde que les centres
        if (i > l-1 && i < l*h-l && i%l!=0 && (i+1)%l!=0){
            unsigned int correspondingSide = 0;
            unsigned int rotationNeeded = 0;
            unsigned int indexBestPiece = 0;
            //std::cout << _sol[i-1].couleur[abs(3-_sol[i-1].rotation)] << " " << _sol[i-l].couleur[abs(0-_sol[i-l].rotation)] << " " << _sol[i+1].couleur[abs(1-_sol[i+1].rotation)] << " " << _sol[i+l].couleur[abs(2-_sol[i-l].rotation)] << " "  << std::endl;
            for (int c = 0; c < centresPermutation.size(); ++c) {


                for (int j = 0; j < 4; ++j) {
                    if (_sol[i-1].couleur[abs(3-_sol[i-1].rotation)]!=0 && _sol[i-1].couleur[abs(3-_sol[i-1].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
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
                        if (_sol[i+1].couleur[abs(1-_sol[i+1].rotation)]!=0 && _sol[i+1].couleur[abs(1-_sol[i+1].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
                            if (correspondingSide < 2){
                                correspondingSide = 2;
                                rotationNeeded = 3-j;
                                indexBestPiece = c;
                            }
                            if (j==4){
                                j=0;
                            }
                            else {
                                ++j;
                            }
                            if (_sol[i-l].couleur[abs(0-_sol[i-l].rotation)]!=0 && _sol[i-l].couleur[abs(0-_sol[i-l].rotation)]==problem.centres[centresPermutation[c]].couleur[j]){
                                correspondingSide = 3;
                                if (2-j == -1){
                                    rotationNeeded = 3;
                                }else{
                                    rotationNeeded = 2-j;
                                }
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

    //on replace les indices au bon endroit
/*
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
*/
}