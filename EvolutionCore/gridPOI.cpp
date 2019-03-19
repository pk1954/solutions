// gridPOI.cpp
//

#include "stdafx.h"
#include "EvolutionCoreImpl.h"
#include "gridPOI.h"

IND_ID GridPOI::m_idPOI = IND_ID::NULL_VAL();

void GridPOI::SetPoi( IND_ID const idNew )       
{ 
	if ( idNew.IsNotNull( ) )
	{    
		if ( idNew == m_idPOI )
			ClearPoi( );           // same POI. deactivate POI
		else
			m_idPOI = idNew; 
	}
}
