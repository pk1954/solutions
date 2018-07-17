// EvolutionModelDataImpl.cpp
//

#include "stdafx.h"
#include "EvolutionModelDataImpl.h"

void EvolutionModelDataImpl::CopyEvolutionModelData( EvolutionModelData const * const src )
{
    * this = * static_cast<EvolutionModelDataImpl const *>( src );
}

GridPoint EvolutionModelDataImpl::FindPOI( ) const 
{ 
	return IsPoiDefined( ) 
			? FindGridPoint( m_idPOI ) 
			: GridPoint::GP_NULL; 
}

void EvolutionModelDataImpl::SetPoi( GridPoint const & gp )       
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

GridPoint EvolutionModelDataImpl::FindGridPoint( IndId const & id ) const 
{ 
	return m_grid.FindGridPoint( [&](GridPoint const & gp) { return (GetId(gp) == id); } );
}
