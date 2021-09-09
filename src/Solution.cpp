#include "Solution.h"

void Solution::printOn(std::ostream& os) const{
	for(unsigned int i=0; i< size(); i++){
		os << operator[](i).id << std::endl;
	}
}

void Solution::readFrom(std::istream& is){

}

Solution::Solution(const Solution& _s):EO<eoMinimizingFitness>(_s), std::vector <struct Piece>(_s){}


Solution::Solution(){}

Solution& Solution::operator=(const Solution& _sol){
	if(&_sol != this){
		(*this).resize(0);
		for(unsigned int i=0; i<_sol.size(); i++)
			(*this).push_back(_sol[i]);
	}
	return *this;
}
