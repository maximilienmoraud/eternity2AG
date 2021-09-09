#include "Solution.h"
#include "Problem.h"
#include "utils/eoRNG.h"
#include "eoOp.h"

class swapRotate: public eoMonOp<Solution>
{
 public:

	swapRotate(Problem& _problem):problem(_problem){}

	bool operator()(Solution& _sol){

		std::pair <unsigned int, unsigned int> p;
		std::vector<unsigned int> coinsPermutation;
		std::vector<unsigned int> bordsPermutation;
		std::vector<unsigned int> centresPermutation;
		unsigned int l=problem.taille.first;
		unsigned int h=problem.taille.second;
		unsigned int gain;
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


		//swap des bords
		//TODO Gerer les indices

		for(unsigned int i=0; i<1; i+=2){
			unsigned int cas=floor(rng.uniform(256));

			if(cas<4){
				p.first=problem.idCoins[(coinsPermutation[i])%problem.idCoins.size()];
				p.second=problem.idCoins[(coinsPermutation[i+1])%problem.idCoins.size()];
			}
			else if(cas<60){
				p.first=problem.idBords[(coinsPermutation[i])%problem.idBords.size()];
				p.second=problem.idBords[(coinsPermutation[i+1])%problem.idBords.size()];
			}
			else{
//				p.first=problem.idCentres[(centresPermutation[i])%problem.idCentres.size()];
//				p.second=problem.idCentres[(centresPermutation[i+1])%problem.idCentres.size()];

				unsigned int k=0;
				unsigned int bon=0;
				unsigned int tmp=0;
				std::pair <unsigned int, unsigned int> best;
				bool find1=false;
				bool find2=false;
				while((!find1 || !find2) && bon!=4){
					if(!find1){
						if(scorePiece(_sol, problem.idCentres[centresPermutation[k]]) == bon)
							find1=true;
						p.first=problem.idCentres[centresPermutation[k]];
					}
					else{
						best.first=0;
						best.second=0;
						for(unsigned int j=0; j<problem.idCentres.size(); j++){
							if(j!=p.first){
								tmp=scoreReplacePiece(_sol, p.first, problem.idCentres[centresPermutation[j]]);
								if(best.second <= tmp){
									best.second=tmp;
									best.first=j;
								}
							}
						}
						p.second=problem.idCentres[centresPermutation[best.first]];

//						if(scorePiece(_sol, problem.idCentres[centresPermutation[k]]) == bon)
//							find2=true;
//						p.second=problem.idCentres[centresPermutation[k]];
					}
					if(k>=problem.idCentres.size()){
						bon++;
						k=0;
					}
					else
						k++;
				}
			}



	//		std::cout << "Indices Coins : ";
	//		for(unsigned int i=0; i<problem.idCoins.size(); i++)
	//			std::cout << problem.idCoins[i] << " ";
	//
	//		std::cout << "Indices Bords : ";
	//		for(unsigned int i=0; i<problem.idBords.size(); i++)
	//			std::cout << problem.idBords[i] << " ";
	//
	//		std::cout << "Indices Centres : ";
	//		for(unsigned int i=0; i<problem.idCentres.size(); i++)
	//			std::cout << problem.idCentres[i] << " ";

			//std::cout << "swap: " << p.first << ", " << p.second << std::endl;
			struct Piece tmp=_sol[p.first];
			_sol[p.first]=_sol[p.second];
			_sol[p.second]=tmp;
			if(cas<60)
				_sol[p.second].rotation=_sol[p.first].rotation;
			else{
				_sol[p.second].rotation=bestRotation(_sol, p.second);
			}
			_sol[p.second].position=_sol[p.first].position;
			if(cas<60)
				_sol[p.first].rotation=tmp.rotation;
			else{
				_sol[p.first].rotation=bestRotation(_sol, p.first);
			}
			_sol[p.first].position=tmp.position;
		}

		//Tout plein de rotations
//		for(unsigned int i=0; i<problem.idCentres.size(); i++){
//			_sol[problem.idCentres[i]].rotation=bestRotation(_sol,problem.idCentres[i]);
//		}

		_sol.invalidate();
		return true;
	}


 private:
	Problem& problem;

	unsigned int bestRotation(Solution& _sol, unsigned int _id){
		unsigned int res=0;
		unsigned int gain=0;
		unsigned int best=0;
		for(unsigned int i=0; i<4; i++){
			if(_sol[_id-1].couleur[3-_sol[_id-1].rotation] == (_sol[_id].couleur[5-i])%4)
				gain++;
			if(_sol[_id+1].couleur[(5-_sol[_id+1].rotation)%4] == _sol[_id].couleur[3-i])
				gain++;
			if(_sol[_id-problem.taille.first].couleur[(4-_sol[_id-problem.taille.first].rotation)%4] == (_sol[_id].couleur[6-i])%4)
				gain++;
			if(_sol[_id+problem.taille.first].couleur[(3-_sol[_id+problem.taille.first].rotation)%4] == (_sol[_id].couleur[4-i])%4)
				gain++;
			if(gain > best){
				best=gain;
				res=i;
			}
			gain=0;
		}
		//std::cout << "Mut best gain: " << best << std::endl;
		return res;
	}

	unsigned int scorePiece(Solution& _sol, unsigned int _id){
		unsigned int res=0;

		if(_sol[_id-1].couleur[3-_sol[_id-1].rotation] == (_sol[_id].couleur[5-_sol[_id].rotation])%4)
			res++;
		if(_sol[_id+1].couleur[(5-_sol[_id+1].rotation)%4] == _sol[_id].couleur[3-_sol[_id].rotation])
			res++;
		if(_sol[_id-problem.taille.first].couleur[(4-_sol[_id-problem.taille.first].rotation)%4] == (_sol[_id].couleur[6-_sol[_id].rotation])%4)
			res++;
		if(_sol[_id+problem.taille.first].couleur[(3-_sol[_id+problem.taille.first].rotation)%4] == (_sol[_id].couleur[4-_sol[_id].rotation])%4)
			res++;
		//std::cout << "score Piece : " << res << std::endl;
		return res;
	}

	unsigned int scoreReplacePiece(Solution& _sol, unsigned int _id, unsigned int _replace){
		unsigned int gain=0;
		unsigned int best=0;
		for(unsigned int i=0; i<4; i++){
			if(_sol[_id-1].couleur[3-_sol[_id-1].rotation] == (_sol[_replace].couleur[5-i])%4)
				gain++;
			if(_sol[_id+1].couleur[(5-_sol[_id+1].rotation)%4] == _sol[_replace].couleur[3-i])
				gain++;
			if(_sol[_id-problem.taille.first].couleur[(4-_sol[_id-problem.taille.first].rotation)%4] == (_sol[_replace].couleur[6-i])%4)
				gain++;
			if(_sol[_id+problem.taille.first].couleur[(3-_sol[_id+problem.taille.first].rotation)%4] == (_sol[_replace].couleur[4-i])%4)
				gain++;
			if(gain > best){
				best=gain;
			}
			gain=0;
		}
		return best;
	}
};
