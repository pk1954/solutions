// PixelCore.cpp
//

#include "stdafx.h"
#include "gridPoint.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "PixelCore.h"

PixelCore::PixelCore
( 
    EvolutionCore      * pCore,
    EvolutionModelData * pModel,
	PixelCoordinates   * pPixelCoordinates
)
{
	m_pCore             = pCore;
	m_pModelWork        = pModel;
	m_pPixelCoordinates = pPixelCoordinates;
}

PixelCore::~PixelCore( )
{
	m_pModelWork        = nullptr;
	m_pCore             = nullptr;
	m_pPixelCoordinates = nullptr;
}

void PixelCore::SetSelection( PixelPoint const & pt1, PixelPoint const & pt2 )
{
    PixelRect const rectSelection( pt1, pt2 );	   // Current selection
    GridRect        rect( m_pPixelCoordinates->Pixel2GridRect( rectSelection ) );
    rect.ClipToGrid( );
	m_pModelWork->SetSelection( rect );
}

PixelPoint PixelCore::GetPoiCenter() const
{
	GridPoint  const gpPoi       = m_pCore->FindPOI( m_pModelWork );
    PixelPoint const pixPointPoi = m_pPixelCoordinates->Grid2PixelPosCenter( gpPoi );
	return pixPointPoi;
}

void PixelCore::SetPOI( PixelPoint const pixPos ) const
{
    GridPoint const gpPoiNew = m_pPixelCoordinates->Pixel2GridPos( pixPos );
    if ( gpPoiNew.IsInGrid( ) )
    {
        IndId const idPoiNew = m_pModelWork->GetId( gpPoiNew );
        if ( idPoiNew.IsDefined( ) )
        {    
            if ( m_pCore->IsPoiId( idPoiNew ) )
                m_pCore->ClearPoi( );           // same POI. deactivate POI
            else
                m_pCore->SetPoi( m_pModelWork, gpPoiNew );
        }
    }
}

PixelPoint PixelCore::GetNewCenter( PixelPoint const pntCenter ) const
{
	return m_pCore->IsPoiDefined( ) 
	       ? GetPoiCenter( )
	       : pntCenter;
}

bool PixelCore::CenterPoi( PixelPoint const pntCenter ) const
{
	return m_pPixelCoordinates->CenterPoi
		   ( 
			  pntCenter, 
			  m_pCore->FindPOI( m_pModelWork )
		   );
}

void PixelCore::FitToRect( PixelRectSize const rectSize ) const
{
	m_pPixelCoordinates->FitGridToRect
	( 
		m_pModelWork->GetSelection(),
		rectSize
	);
	m_pModelWork->ResetSelection( );
}

void PixelCore::SetFieldSize( short const sNewFieldSize, PixelPoint const pntCenter )
{
    (void)m_pPixelCoordinates->SetGridFieldSize
	( 
		sNewFieldSize, 
		GetNewCenter( pntCenter ) 
	);
}
