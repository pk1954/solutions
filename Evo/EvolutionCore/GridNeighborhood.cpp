// GridNeighborhood.cpp :
//
// EvoCoreLib

module EvoCoreLib:GridNeighborhood;

import :GridPoint;
import :GridRect;
import :GridModel;

int GridNeighborhood::m_iNrOfNeighbors = 0;

void GridNeighborhood::InitClass(int const iNrOfNeighbors) // Initialization of m_pGridNeighbors
{
	Assert((iNrOfNeighbors == 4) || (iNrOfNeighbors == 6) || (iNrOfNeighbors == 8));
	m_iNrOfNeighbors = iNrOfNeighbors;
}
