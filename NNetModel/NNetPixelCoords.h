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

	PixelPoint NNet2PixelSize( NNetPoint const np ) const
	{ 
		auto pixX = PIXEL( CastToLong( np.GetXvalue() / m_nmPixelSize.GetValue() ) );
		auto pixY = PIXEL( CastToLong( np.GetYvalue() / m_nmPixelSize.GetValue() ) );

		return PixelPoint( pixX, pixY );
	}

	NNetPoint Pixel2NNetPos ( PixelPoint const pp ) const
	{ 
		auto pixPoint = PixelPoint( pp + m_pixOffset );

		auto np = NNetPoint
		( 
			nm( pixPoint.GetXvalue() * m_nmPixelSize.GetValue()), 
			nm( pixPoint.GetYvalue() * m_nmPixelSize.GetValue()) 
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

	PIXEL      ComputeNewFieldSize( bool const ) const;  // does not modify field size

	//////// manipulation functions ////////

	bool SetPixelSize( nm const nmPixelSize )
	{
		m_nmPixelSize = nmPixelSize;
	}

	void MoveNNet( PixelPoint const pntDelta )
	{
		m_pixOffset -= pntDelta;
	}

private:

	bool isValidFieldSize( nm const nmNewPixelSize ) const
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
