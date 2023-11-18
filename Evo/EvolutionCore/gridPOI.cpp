// GridPOI.cpp
//


module;

#include "EvolutionCoreImpl.h"

module GridPOI;

GridPoint GridPOI::m_gpPOI = GP_NULL;

void GridPOI::SetPoi(GridPoint const gp)       
{ 
	if (IsPoi(gp))
		ClearPoi();           // same POI. deactivate POI
	else
		m_gpPOI = gp; 
}
