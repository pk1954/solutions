// EvolutionModelDataImpl.cpp
//

#include "stdafx.h"
#include "EvolutionModelDataImpl.h"

void EvolutionModelDataImpl::ResetAll( )
{
    m_grid.ResetGrid( );
	m_idPOI.ResetIndId( );
	m_plan.SetInvalid( );
	ResetSelection( );
	m_brush.Reset( );
	m_bSimulationMode = false;
}

void EvolutionModelDataImpl::CopyEvolutionModelData( EvolutionModelData const * const src )
{
	* this = * static_cast<EvolutionModelDataImpl const *>( src );
}

void EvolutionModelDataImpl::SetPoi( GridPoint const &  gp )       
{ 
	if ( gp.IsInGrid( ) )
	{
		IndId const idPoiNew = GetId( gp );
		if ( idPoiNew.IsDefined( ) )
		{    
			if ( IsPoiId( idPoiNew ) )
				ClearPoi( );           // same POI. deactivate POI
			else
				m_idPOI = m_grid.GetId( gp ); 
		}
	}
}

GridPoint EvolutionModelDataImpl::FindPOI( ) const 
{ 
	return IsPoiDefined( ) 
			? FindGridPoint( m_idPOI ) 
			: GridPoint::GP_NULL; 
}

GridPoint EvolutionModelDataImpl::FindGridPoint( IndId const & id ) const 
{ 
	return m_grid.FindGridPoint( [&](GridPoint const & gp) { return (GetId(gp) == id); } );
}
