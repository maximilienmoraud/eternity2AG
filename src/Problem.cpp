#include "Problem.h"
#include<fstream>
#include<iostream>

Problem::Problem(): nbIndice(0), nbCouleur(0){
	taille.first=0;
	taille.second=0;
	all.resize(0);
	coins.resize(0);
	bords.resize(0);
	centres.resize(0);
	statCouleur.resize(0);
}

void Problem::load(std::string _bench){
	std::cout << "load bench: " << _bench << std::endl;
	unsigned int couleurBas;
	unsigned int couleurGauche;
	unsigned int couleurHaut;
	unsigned int couleurDroite;
	unsigned int couleurMax=0;
	unsigned int nbIndice;
	
	unsigned int id, find;
	
	struct Piece piece;

	std::ifstream infile(_bench.c_str());
	if (!infile.is_open())
		std::cout << "Impossible d'ouvrir le benchmark: " << _bench << std::endl;
	else{
		infile >> taille.first >> taille.second >> nbIndice;
		for(unsigned int i=0; i<nbIndice; i++){
			infile >> id;
			piece.id=id;
			infile >> couleurHaut >> couleurGauche;
			infile >> couleurDroite;
			piece.rotation=couleurDroite;
			piece.position=taille.first*couleurHaut + couleurGauche; // verifier pour le first et second
			piece.fixe=true;
			indices.push_back(piece);
		}
		id=1;


		while(!infile.eof()){
			infile >> couleurBas;
			infile >> couleurGauche;
			infile >> couleurHaut;
			infile >> couleurDroite;
		if(couleurBas > couleurMax)
			couleurMax=couleurBas;
		if(couleurGauche > couleurMax)
			couleurMax=couleurGauche;
		if(couleurHaut > couleurMax)
			couleurMax=couleurHaut;
		if(couleurDroite > couleurMax)
			couleurMax=couleurDroite;

			if(!infile.eof()){
				//Pour afficher le fichier pars� decommenter la ligne ci dessous
				//std::cout << couleurBas << ", " << couleurGauche << ", " << couleurHaut << ", " << couleurDroite << std::endl;
				if(estUnIndice(id)){
					find=0;
					while(indices[find].id !=id)
						find++;
					indices[find].couleur[0]=couleurBas;
					indices[find].couleur[1]=couleurGauche;
					indices[find].couleur[2]=couleurHaut;
					indices[find].couleur[3]=couleurDroite;
					piece.position=indices[find].position;
					piece.rotation=indices[find].rotation;
					piece.fixe=true;
				}
				else{
					piece.position=-1;
					piece.rotation=-1;
					piece.fixe=false;
				}
				piece.id=id;
				piece.couleur[0]=couleurBas;
				piece.couleur[1]=couleurGauche;
				piece.couleur[2]=couleurHaut;
				piece.couleur[3]=couleurDroite;

				all.push_back(piece);
				//on regarde si on a un coin
				if((couleurBas==0) && (couleurGauche==0)){
					piece.nbBord=2;
					coins.push_back(piece);
				}
				//sinon on regarde si on a un bord
				else if(couleurBas==0){
					piece.nbBord=1;
					bords.push_back(piece);
				}
				//sinon on a une piece centrale
				else{
					piece.nbBord=0;
					centres.push_back(piece);
				}
				id++;
			}
		}
	}
	//Compte des couleurs avec un modulo 10000 quand les couleurs font parti du contours (les 4 faces des coins et les face Bas, Gauche et Droite des bords)
	statCouleur.resize(couleurMax+1);
	for(unsigned int i=0; i< statCouleur.size(); i++){
		statCouleur[i].first=0;
		statCouleur[i].second=0;
	}

	for(unsigned int i=0; i< coins.size(); i++)
		for(unsigned int j=0; j<4 ; j++)
			statCouleur[coins[i].couleur[j]].first+=10000;

	for(unsigned int i=0; i< bords.size(); i++)
		for(unsigned int j=0; j<4 ; j++)
			if(j!=2)
				statCouleur[bords[i].couleur[j]].first+=10000;
			else{
				statCouleur[bords[i].couleur[j]].first++;
				statCouleur[bords[i].couleur[j]].second++;
			}

	for(unsigned int i=0; i< centres.size(); i++)
		for(unsigned int j=0; j<4 ; j++)
			statCouleur[centres[i].couleur[j]].first++;

	//----------------------------------------------------------------
	//COPIE DES PIECES SUIVANT LES COULEURS.
	nbCouleur=0;
	for(unsigned int i=0; i<all.size(); i++){
		if(all[i].couleur[0]> nbCouleur)
			nbCouleur=all[i].couleur[0];
		if(all[i].couleur[1]> nbCouleur)
			nbCouleur=all[i].couleur[1];
		if(all[i].couleur[2]> nbCouleur)
			nbCouleur=all[i].couleur[2];
		if(all[i].couleur[3]> nbCouleur)
			nbCouleur=all[i].couleur[3];
	}
	nbCouleur++;
//	std::cout << "nombre de couleur: " << nbCouleur << std::endl;
	tabCouleur.resize(nbCouleur);
	for(unsigned int i=0; i<all.size(); i++){
		tabCouleur[all[i].couleur[0]].push_back(all[i]);
		if(all[i].couleur[1] != all[i].couleur[0]){
			tabCouleur[all[i].couleur[1]].push_back(all[i]);
		}
		if((all[i].couleur[2] != all[i].couleur[0]) && (all[i].couleur[2] != all[i].couleur[1])){
			tabCouleur[all[i].couleur[2]].push_back(all[i]);
		}
		if((all[i].couleur[3] != all[i].couleur[0]) && (all[i].couleur[3] != all[i].couleur[1]) && (all[i].couleur[3] != all[i].couleur[2])){
			tabCouleur[all[i].couleur[3]].push_back(all[i]);
		}
	}

//	for(unsigned int i=0; i<tabCouleur.size(); i++){
//		std::cout << "Couleur " << i << ": ";
//		for(unsigned int j=0; j<tabCouleur[i].size(); j++)
//			std::cout << tabCouleur[i][j].id << " ";
//		std::cout << std::endl;
//	}


	//----------------------------------------------------------------
	//ON REMPLI LES VECTEURS D'INDICES

	for(unsigned int i=0; i<taille.first*taille.second; i++){
		if(i==0 || i==taille.first-1 || i==taille.first*taille.second-1 || i== taille.first*(taille.second-1))
			idCoins.push_back(i);
		else if(i<taille.first || i%taille.first==0 || i%taille.first==taille.first-1 || i>taille.first*(taille.second-1))
			idBords.push_back(i);
		else
			idCentres.push_back(i);

	}

}

bool Problem::estUnIndice(unsigned int _id){
	bool res=false;
	unsigned int i=0;
	while(!res && i<indices.size()){
		if(indices[i].id==_id)
			res=true;
		i++;
	}
	return res;
}

unsigned int Problem::positionIndice(unsigned int _id){
	unsigned int res=0;
	unsigned int i=0;
	bool stop=false;
	while(!stop && i<indices.size()){
		if(indices[i].id==_id)
			res=indices[i].position;
		i++;
	}
	return res;
}

unsigned int Problem::rotationIndice(unsigned int _id){
	unsigned int res=0;
	unsigned int i=0;
	bool stop=false;
	while(!stop && i<indices.size()){
		if(indices[i].id==_id)
			res=indices[i].rotation;
		i++;
	}
	return res;
}

void Problem::printPiece(struct Piece _piece){
	std::cout << "Piece " << _piece.id;
	if(_piece.nbBord==2)
		std::cout << " (Coin)";
	else if(_piece.nbBord==1)
		std::cout << " (Bord)";
	if(_piece.fixe)
		std::cout << " -> Piece Indice";
	std::cout << std::endl;
	std::cout << "Couleur (Bas, Gauche, Haut, Droite) -> " << _piece.couleur[0] << ", " << _piece.couleur[1] << ", " <<_piece.couleur[2] << ", " <<_piece.couleur[3] << std::endl;
	if(_piece.position==-1)
		std::cout << "Non positionnee" << std::endl;
	else{
		if(_piece.fixe)
			std::cout << "Position Fixe: " << _piece.position << ", Rotation (Sens Horraire): " << _piece.rotation << std::endl;
		else
			std::cout << "Position: " << _piece.position << ", Rotation (Sens Horraire): " << _piece.rotation << std::endl;
	}
	std::cout << std::endl;
}

void Problem::printCoins(){
	std::cout << "Liste des Coins" << std::endl;
	std::cout << "---------------" << std::endl;
	for(unsigned int i=0; i< coins.size(); i++)
		printPiece(coins[i]);
}

void Problem::printBords(){
	std::cout << "Liste des Bords" << std::endl;
	std::cout << "---------------" << std::endl;
	for(unsigned int i=0; i< bords.size(); i++)
		printPiece(bords[i]);
}

void Problem::printCentres(){
	std::cout << "Liste des Pieces centrales" << std::endl;
	std::cout << "--------------------------" << std::endl;
	for(unsigned int i=0; i< centres.size(); i++)
		printPiece(centres[i]);
}

void Problem::printIndices(){
	std::cout << "Liste des Indices" << std::endl;
	std::cout << "--------------------------" << std::endl;
	for(unsigned int i=0; i< indices.size(); i++)
		printPiece(indices[i]);
}

void Problem::print(){
	std::cout << "Instance de taille: " << taille.first << " * " << taille.second <<std::endl;
	printCoins();
	printBords();
	printCentres();
	printIndices();
}

void Problem::infoCouleur(){
	std::cout << "Liste des Couleurs ([Countours, Centres dont X Coutours Interieur])" << std::endl;
	for (unsigned int i=0; i< statCouleur.size(); i++)
		std::cout << i << ": [" << statCouleur[i].first/10000 << ", " << statCouleur[i].first%10000 << " dont " <<statCouleur[i].second << " Coutours Interieur]" << std::endl;
}

void Problem::printSol(const Solution& _sol){
	unsigned int index, cpt, couleur1, couleur2, couleurSuivante, couleurDessous;
	cpt=0;
	for(unsigned int i=0; i<taille.first; i++){
		for(unsigned int j=0; j<taille.second; j++){
			index=i*taille.first + j;
			couleur1=_sol[index].couleur[(6-_sol[index].rotation)%4];
			if(couleur1 > 9)
				std::cout << "\\ "<< couleur1 << " / | ";
			else
				std::cout << "\\  " << couleur1 << " / | ";
		}
		std::cout << std::endl;
		for(unsigned int j=0; j<taille.second; j++){
			index=i*taille.first + j;
			couleur1=_sol[index].couleur[(5-_sol[index].rotation)%4];
			couleur2=_sol[index].couleur[(3-_sol[index].rotation)%4];
			if(j<taille.second-1)
				couleurSuivante=_sol[index+1].couleur[(5-_sol[index+1].rotation)%4];
			else
				couleurSuivante=0;
			if(couleur1 > 9)
				std::cout << couleur1 << "  ";
			else
				std::cout << couleur1 << "   ";
			if(couleur2 > 9)
				std::cout << couleur2;
			else
				std::cout << " " << couleur2;
			if(couleur2 == couleurSuivante)
				std::cout << " | ";
			else{
				std::cout << " \033[31mX\033[0m ";
				cpt++;
			}
		}
		std::cout << std::endl;
		for(unsigned int j=0; j<taille.second; j++){
			index=i*taille.first + j;
			couleur1=_sol[index].couleur[(4-_sol[index].rotation) %4];
			if(couleur1 > 9)
				std::cout << "/ "<< couleur1 << " \\ | ";
			else
				std::cout << "/  "<< couleur1 << " \\ | ";
		}
		std::cout << std::endl;
		for(unsigned int j=0; j<taille.second; j++){
			index=i*taille.first + j;
			couleur1=_sol[index].couleur[(4-_sol[index].rotation) %4];
			if(i<taille.first-1)
				couleurDessous=_sol[index+taille.second].couleur[(6-_sol[index+taille.second].rotation) %4];
			else
				couleurDessous=0;
			if(couleur1==couleurDessous)
				std::cout << "---------";
			else{
				std::cout << "---\033[31mX\033[0m-----";
				cpt++;
			}
		}
		std::cout << std::endl;
	}

	std::cout << cpt << " erreurs" << std::endl;
}

void Problem::printSolinFile(const Solution& _sol, std::string _outputFileName){

	std::ofstream file;
    char buffer [80];
    strcpy(buffer, _outputFileName.c_str());
  file.open(buffer);
if (file.is_open())
  {
  	//std::cout << "File open" << std::endl;
  	file.clear();
  }
	else
	{
		std::cout << "Unable to open file" << std::endl;
	}
  
  unsigned int index, c1, c2, c3, c4;
  //std::cout << taille.first << "\t" << taille.second << std::endl;
  file << taille.first << "\t" << taille.second << std::endl;
  //std::cout << statCouleur.size() << std::endl;
  file << statCouleur.size() << std::endl;
  for(unsigned int i=0; i<taille.first; i++){
    for(unsigned int j=0; j<taille.second; j++){
      index=i*taille.first + j;
      c1=_sol[index].couleur[(6-_sol[index].rotation)%4];
      c2=_sol[index].couleur[(5-_sol[index].rotation)%4];
      c3=_sol[index].couleur[(4-_sol[index].rotation)%4];
      c4=_sol[index].couleur[(3-_sol[index].rotation)%4];
      //std::cout << c1 << "\t" << c2 << "\t" << c3 << "\t" << c4 << std::endl;
      file << c1 << "\t" << c2 << "\t" << c3 << "\t" << c4 << std::endl;
    }
  }
  file.close();
  std::cout << "File : " << _outputFileName << " written" << std::endl;
}

const std::vector <struct Piece>& Problem::getCoins(){
	return coins;
}

const std::vector <struct Piece>& Problem::getBords(){
	return bords;
}

const std::vector <struct Piece>& Problem::getCentres(){
	return centres;
}
