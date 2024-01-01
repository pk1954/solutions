// GridNeighbor.cpp :
//
// EvolutionCore

module;

#include "grid_model.h"

module GridNeighbor;

import GridPoint;
import GridRect;

int Neighborhood::m_iNrOfNeighbors = 0;

void Neighborhood::InitClass(int const iNrOfNeighbors) // Initialization of m_pGridNeighbors
{
	assert((iNrOfNeighbors == 4) || (iNrOfNeighbors == 6) || (iNrOfNeighbors == 8));
	m_iNrOfNeighbors = iNrOfNeighbors;
}
