// gridPOI.cpp
//


#include "EvolutionCoreImpl.h"
#include "gridPOI.h"

GridPoint GridPOI::m_gpPOI = GP_NULL;

void GridPOI::SetPoi( GridPoint const gp )       
{ 
	if ( IsPoi( gp ) )
		ClearPoi( );           // same POI. deactivate POI
	else
		m_gpPOI = gp; 
}
