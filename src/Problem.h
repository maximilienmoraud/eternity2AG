#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include "Piece.h"
#include "Solution.h"

class Problem{

public :

	/**
	 * Constructeur par défaut
	 */
	Problem();

	/**
	 * Charge un benchmark
	 * @param _bench le benchmarch à charger
	 */
	void load(std::string _bench);

	/**
	 * Affiche une piece
	 * @param _piece la piece à afficher
	 */
	void printPiece(struct Piece _piece);

	/**
	 * Affiche la liste des coins
	 */
	void printCoins();

	/**
	 * Affiche la liste des bords
	 */
	void printBords();

	/**
	 * Affiche la liste des pieces centrales
	 */
	void printCentres();

	/**
	 * Affiche les indices
	 */
	void printIndices();

	/**
	 * Affiche toutes les pieces
	 */
	void print();

	/**
	 * Affiche des stats sur la repartition des couleurs du probleme
	 */
	void infoCouleur();

	/**
	 * Affiche une solution sous forme d'une grille avec indication des erreurs
	 * @param _sol une reference vers une solution
	 */
	void printSol(const Solution& _sol);

	/**
	 * Ecrit dans un fichier une solution avec les 4 couleurs de chaques pieces
	 * @param _sol une reference vers une solution
	 * @param _outputFileName le chemin du fichier de sortie
	 */
	void printSolinFile(const Solution& _sol, std::string _outputFileName);

	/**
	 * Test si la piece est un indice
	 * @param _id l'id de la piece
	 * @return true si la piece est un indice
	 */
	bool estUnIndice(unsigned int _id);

	/**
	 * Retourne la position d'un indice en fonction de son id
	 * @param _id l'id de l'indice
	 * @return la position de l'indice
	 */
	unsigned int positionIndice(unsigned int _id);

	/**
	 * Retourne la rotation d'un indice en fonction de son id
	 * @param _id l'id de l'indice
	 * @return la rotation de l'indice
	 */
	unsigned int rotationIndice(unsigned int _id);

	/**
	 * Accesseur sur les coins
	 * @return le vecteur des coins
	 */
	const std::vector <struct Piece>& getCoins();

	/**
	 * Accesseur sur les bords
	 * @return le vecteur des bords
	 */
	const std::vector <struct Piece>& getBords();

	/**
	 * Accesseur sur les pieces centrales
	 * @return le vecteur des pieces centrales
	 */
	const std::vector <struct Piece>& getCentres();
	
	unsigned int nbIndice;
	std::vector <struct Piece> all;
	std::vector <struct Piece> coins;
	std::vector<struct Piece> bords;
	std::vector<struct Piece> centres;

	std::vector< std::vector<struct Piece> > tabCouleur;

	//les indices sont egalement dans coins, bords ou centres
	std::vector <struct Piece> indices;

	std::vector <unsigned int> idCoins;
	std::vector <unsigned int> idBords;
	std::vector <unsigned int> idCentres;

	//stats
	std::pair <unsigned int, unsigned int> taille;
	unsigned int nbCouleur;
	std::vector < std::pair<unsigned int, unsigned int> > statCouleur;
};
#endif

