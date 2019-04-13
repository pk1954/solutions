// gridNeighbor.cpp :
//

#include "stdafx.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "grid_model.h"
#include "gridNeighbor.h"

int Neighborhood::m_iNrOfNeighbors = 0;

void Neighborhood::InitClass( int const iNrOfNeighbors ) // Initialization of m_pGridNeighbors
{
	assert( ( iNrOfNeighbors == 4 ) || ( iNrOfNeighbors == 6 ) || ( iNrOfNeighbors == 8 ) );
	m_iNrOfNeighbors = iNrOfNeighbors;
}
