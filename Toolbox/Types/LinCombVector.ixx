// LinCombVector.ixx
//
// Toolbox\Types

module;

#include <vector>

export module Types:LinCombVector;

using std::vector;

export template <typename T>
struct LinCombVector : vector<T>
{
    LinCombVector& operator+= (LinCombVector const &rhs) 
    { 
        for (int i = 0; i < this->size(); ++i)
	        (*this)[i] += rhs[i];
        return *this; 
    }

    LinCombVector& operator-= (LinCombVector const &rhs) 
    { 
        for (int i = 0; i < this->size(); ++i)
	        (*this)[i] -= rhs[i];
        return *this; 
    }

    LinCombVector& operator*= (float const factor) 
    { 
        for (int i = 0; i < this->size(); ++i)
	        (*this)[i] *= factor;
        return *this; 
    }
};
