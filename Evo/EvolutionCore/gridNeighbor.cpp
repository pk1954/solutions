// GridNeighbor.cpp :
//
// EvolutionCore

module EvolutionCore:;

#include "grid_model.h"

module EvolutionCore:GridNeighbor;

import :GridPoint;
import :GridRect;

int Neighborhood::m_iNrOfNeighbors = 0;

void Neighborhood::InitClass(int const iNrOfNeighbors) // Initialization of m_pGridNeighbors
{
	Assert((iNrOfNeighbors == 4) || (iNrOfNeighbors == 6) || (iNrOfNeighbors == 8));
	m_iNrOfNeighbors = iNrOfNeighbors;
}
