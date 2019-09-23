// pixelCoordinates.h : 
//
// NNetModel

#pragma once

#include "util.h"
#include "PixelTypes.h"
#include "NNetPoint.h"
#include "SmoothMove.h"

NanoMeter const MINIMUM_PIXEL_SIZE =     100.0_NanoMeter;
NanoMeter const DEFAULT_PIXEL_SIZE =    1000.0_NanoMeter;  // 1 um
NanoMeter const MAXIMUM_PIXEL_SIZE = 2000000.0_NanoMeter;  // 2 Meter

class NNetPixelCoords
{
public:

	NNetPixelCoords()
	  : m_pixOffset(  0_PIXEL ),
		m_fPixOffset( 0.0_fPIXEL ),
		m_nmPixelSize( 0.0_NanoMeter ),
		m_bMoving    ( false ),
		m_smoothMove ( )
	{}

	void Start( NanoMeter const nmPixelSize )
	{
		m_nmPixelSize = nmPixelSize;
	}

	//////// transformations ////////

	PIXEL Nm2Pixel( NanoMeter const nmParam ) const
	{ 
		NanoMeter nmRes( nmParam / m_nmPixelSize.GetValue() );
		return PIXEL( CastToLong( nmRes.GetValue() ) );
	}

	fPIXEL Nm2fPixel( NanoMeter const nmParam ) const
	{ 
		NanoMeter nmRes( nmParam / m_nmPixelSize.GetValue() );
		return fPIXEL( nmRes.GetValue() );
	}

	///////////////////

	NanoMeter Pixel2Nm( PIXEL const pixel ) const
	{ 
		return NanoMeter( pixel.GetValue() * m_nmPixelSize.GetValue() );
	}

	NanoMeter fPixel2Nm( fPIXEL const fPixel ) const
	{ 
		return NanoMeter( fPixel.GetValue() * m_nmPixelSize.GetValue() );
	}

	///////////////////

	PixelPoint NNet2PixelSize( NNetPoint const np ) const
	{ 
		return PixelPoint( Nm2Pixel( np.GetX() ), Nm2Pixel( np.GetY() ) );
	}

	fPixelPoint NNet2fPixelSize( NNetPoint const np ) const
	{ 
		return fPixelPoint( Nm2fPixel( np.GetX() ), Nm2fPixel( np.GetY() ) );
	}

	///////////////////

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

	NNetPoint fPixel2NNetPos ( fPixelPoint const pp ) const
	{ 
		auto fPixPoint = fPixelPoint( pp + m_fPixOffset );

		auto np = NNetPoint
		( 
			fPixel2Nm( fPixPoint.GetX() ), 
			fPixel2Nm( fPixPoint.GetY() )
		); 

		return np;
	}

	///////////////////

	PixelPoint NNet2PixelPos( NNetPoint const np ) const
	{ 
		return NNet2PixelSize( np ) - m_pixOffset;
	}

	fPixelPoint NNet2fPixelPos( NNetPoint const np ) const
	{ 
		return NNet2fPixelSize( np ) - m_fPixOffset;
	}

	//////// queries ////////

	NanoMeter  GetPixelSize( )   const { return m_nmPixelSize; };
	
	PixelPoint GetPixelOffset( ) const { return m_pixOffset; }

	fPixelPoint GetfPixelOffset( ) const { return m_fPixOffset; }

	NanoMeter ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		NanoMeter nmNewPixelSize { m_nmPixelSize };
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

	bool SetPixelSize( NanoMeter const nmPixelSize )
	{
		bool bValid = isValidPixelSize( nmPixelSize );
		if ( bValid )
			m_nmPixelSize = nmPixelSize;
		return bValid;
	}

	void MoveNNet( PixelPoint const pntDelta )
	{
		auto fPntDelta = fPixelPoint
		( 
			fPIXEL( static_cast<double>(pntDelta.GetXvalue()) ), 
			fPIXEL( static_cast<double>(pntDelta.GetYvalue()) )
		);
		m_pixOffset  -= pntDelta;
		m_fPixOffset -= fPntDelta;
	}

private:

	bool isValidPixelSize( NanoMeter const nmNewPixelSize ) const
	{
		return (MINIMUM_PIXEL_SIZE <= nmNewPixelSize) && (nmNewPixelSize <= MAXIMUM_PIXEL_SIZE); 
	}

	PixelPoint  m_pixOffset;
	fPixelPoint m_fPixOffset;
	NanoMeter   m_nmPixelSize;
	SmoothMove  m_smoothMove;
	bool        m_bMoving;
};

PixelPoint NNetPixel2PixelSize( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelPoint NNetPixel2PixelPos ( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelRect  NNetPixel2PixelRect( PixelRect  const &, NNetPixelCoords const *, NNetPixelCoords const * );
