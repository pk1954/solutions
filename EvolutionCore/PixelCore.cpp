// PixelCore.cpp
//

#include "stdafx.h"
#include "gridPoint.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "PixelCore.h"

PixelCore::PixelCore
( 
    EvolutionCore    * pCore,
	PixelCoordinates * pPixelCoordinates
)
{
	m_pCore             = pCore;
	m_pPixelCoordinates = pPixelCoordinates;
}

PixelCore::~PixelCore( )
{
	m_pCore             = nullptr;
	m_pPixelCoordinates = nullptr;
}

void PixelCore::SetSelection( PixelPoint const pt1, PixelPoint const pt2 )
{
	m_pCore->SetSelection( m_pPixelCoordinates->Pixel2GridRect( PixelRect( pt1, pt2 ) ) );
}

PixelPoint PixelCore::GetPoiCenter() const
{
    return m_pPixelCoordinates->Grid2PixelPosCenter( m_pCore->FindPOI( ) );
}

bool PixelCore::CenterPoi( PixelPoint const pntCenter ) const
{
	return m_pPixelCoordinates->CenterPoi
		   ( 
			  pntCenter, 
			  m_pCore->FindPOI( )
		   );
}
