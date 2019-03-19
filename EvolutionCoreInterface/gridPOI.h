// gridPOI.h
//

#pragma once

#include "EvolutionTypes.h"
#include "gridPoint.h"

class GridPOI  // point of interest
{
public:
	static void SetPoi( IND_ID const ); 

	static void ClearPoi( ) 
	{
		m_idPOI.Set2Null( ); 
	}

	static bool IsPoiDefined( ) 
	{ 
		return m_idPOI.IsNotNull( ); 
	}

	static bool IsPoi( IND_ID const id ) 
	{ 
		return id == m_idPOI; 
	}

	static IND_ID GetPoi()
	{
		return m_idPOI;
	}

private:
		static IND_ID m_idPOI;
};
