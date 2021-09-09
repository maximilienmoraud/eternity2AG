#include "Solution.h" 
#include "evalSolution.h"

evalSolution::evalSolution(Problem & _problem): problem(_problem){}

void evalSolution::operator()(Solution & _sol){
	double fit=0;

	unsigned int l=problem.taille.first; //largeur
	unsigned int h=problem.taille.second; //hauteur

	//on verifie les coherances sur les lignes
	for(unsigned int i=0; i< h*l-1; i++){
		if(i%l != l-1){
			if(_sol[i].couleur[(3-_sol[i].rotation)%4] == _sol[i+1].couleur[(5-_sol[i+1].rotation)%4])
				fit++;
		}
	}

	//on verifie les coherances sur les colonnes
	for(unsigned int i=0; i<l; i++){
		for(unsigned j=0; j<l*(h-1); j+=l){
			if(_sol[j+i].couleur[(4-_sol[j+i].rotation)%4] == _sol[j+l+i].couleur[(6-_sol[j+l+i].rotation)%4])
				fit++;
		}
	}

	_sol.fitness(-fit);
}




