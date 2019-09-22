// pixelCoordinates.h : 
//
// NNetModel

#pragma once

#include "util.h"
#include "PixelTypes.h"
#include "NNetPoint.h"
#include "SmoothMove.h"

nm const MINIMUM_PIXEL_SIZE =     100.0_nm;
nm const DEFAULT_PIXEL_SIZE =    1000.0_nm;  // 1 um
nm const MAXIMUM_PIXEL_SIZE = 2000000.0_nm;  // 2 Meter

class NNetPixelCoords
{
public:

	NNetPixelCoords()
	  : m_pixOffset( 0_PIXEL ),
		m_nmPixelSize( 0.0_nm ),
		m_bMoving    ( false ),
		m_smoothMove ( )
	{}

	void Start( nm const nmPixelSize )
	{
		m_nmPixelSize = nmPixelSize;
	}

	//////// transformations ////////

	PIXEL Nm2Pixel( nm const nmParam ) const
	{ 
		nm nmRes( nmParam / m_nmPixelSize.GetValue() );
		return PIXEL( CastToLong( nmRes.GetValue() ) );
	}

	nm Pixel2Nm( PIXEL const pixel ) const
	{ 
		return nm( pixel.GetValue() * m_nmPixelSize.GetValue() );
	}

	PixelPoint NNet2PixelSize( NNetPoint const np ) const
	{ 
		return PixelPoint( Nm2Pixel( np.GetX() ), Nm2Pixel( np.GetY() ) );
	}

	NNetPoint Pixel2NNetPos ( PixelPoint const pp ) const
	{ 
		auto pixPoint = PixelPoint( pp + m_pixOffset );

		auto np = NNetPoint
		( 
			Pixel2Nm( pixPoint.GetX() ), 
			Pixel2Nm( pixPoint.GetY() )
		); 

		return np;
	}

	PixelPoint NNet2PixelPos( NNetPoint const np ) const
	{ 
		return NNet2PixelSize( np ) - m_pixOffset;
	}

	//////// queries ////////

	nm         GetPixelSize( )   const { return m_nmPixelSize; };
	PixelPoint GetPixelOffset( ) const { return m_pixOffset; }

	nm         ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		nm nmNewPixelSize { m_nmPixelSize };
		if ( bZoomIn )
		{
			nmNewPixelSize = nmNewPixelSize * 1.3;
		}
		else
		{
			nmNewPixelSize = nmNewPixelSize / 1.3;
		}
		return isValidPixelSize(nmNewPixelSize) ? nmNewPixelSize : m_nmPixelSize;
	}

	//////// manipulation functions ////////

	bool SetPixelSize( nm const nmPixelSize )
	{
		bool bValid = isValidPixelSize( nmPixelSize );
		if ( bValid )
			m_nmPixelSize = nmPixelSize;
		return bValid;
	}

	void MoveNNet( PixelPoint const pntDelta )
	{
		m_pixOffset -= pntDelta;
	}

private:

	bool isValidPixelSize( nm const nmNewPixelSize ) const
	{
		return (MINIMUM_PIXEL_SIZE <= nmNewPixelSize) && (nmNewPixelSize <= MAXIMUM_PIXEL_SIZE); 
	}

	PixelPoint m_pixOffset;
	nm         m_nmPixelSize;
	SmoothMove m_smoothMove;
	bool       m_bMoving;
};

PixelPoint NNetPixel2PixelSize( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelPoint NNetPixel2PixelPos ( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelRect  NNetPixel2PixelRect( PixelRect  const &, NNetPixelCoords const *, NNetPixelCoords const * );
