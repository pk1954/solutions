// PixelCoordsFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "SmoothMove.h"

class PixelCoordsFp
{
public:

	PixelCoordsFp()
	  : m_pixOffset  ( 0_PIXEL ),
		m_fPixOffset ( 0.0_fPIXEL ),
		m_pixelSize  ( DEFAULT_PIXEL_SIZE ),
		m_bMoving    ( false ),
		m_smoothMove ( )
	{}

	void Start( NanoMeter const pixelSize )
	{
		m_pixelSize = pixelSize;
	}

	//////// transformations ////////

	fPIXEL convert2fPixel( MicroMeter const param ) const
	{ 
		MicroMeter res( ( param * 1000.0 ) / m_pixelSize.GetValue() );
		return fPIXEL( res.GetValue() );
	}

	MicroMeter convert2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return MicroMeter( fPixel.GetValue() * m_pixelSize.GetValue() / 1000.0 );
	}

	fPixelPoint convert2fPixelSize( MicroMeterPoint const np ) const
	{ 
		return fPixelPoint( convert2fPixel( np.GetX() ), convert2fPixel( np.GetY() ) );
	}

	MicroMeterPoint convert2MicroMeterPoint( fPixelPoint const pp ) const
	{ 
		auto fPixPoint = fPixelPoint( pp + m_fPixOffset );

		auto np = MicroMeterPoint
		( 
			convert2MicroMeter( fPixPoint.GetX() ), 
			convert2MicroMeter( fPixPoint.GetY() )
		); 

		return np;
	}

	fPixelPoint convert2fPixelPos( MicroMeterPoint const np ) const
	{ 
		return convert2fPixelSize( np ) - m_fPixOffset;
	}

	//////// queries ////////

	NanoMeter   GetPixelSize( )    const { return m_pixelSize; };
	
	fPixelPoint GetfPixelOffset( ) const { return m_fPixOffset; }

	NanoMeter ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		NanoMeter newPixelSize { m_pixelSize };
		if ( bZoomIn )
		{
			newPixelSize = newPixelSize / 1.3;
		}
		else
		{
			newPixelSize = newPixelSize * 1.3;
		}
		return isValidPixelSize(newPixelSize) ? newPixelSize : m_pixelSize;
	}

	PIXEL convert2PIXEL( fPIXEL const fP ) const
	{
		return PIXEL( CastToLong( fP.GetValue() ) );
	}

	fPIXEL convert2fPIXEL( PIXEL const p ) const
	{
		return fPIXEL( static_cast<double>( p.GetValue() ) );
	}

	PixelPoint convert2PixelPoint( fPixelPoint const fPnt ) const
	{
		return PixelPoint( convert2PIXEL(fPnt.GetX()), convert2PIXEL(fPnt.GetY()) );
	}

	fPixelPoint convert2fPixelPoint( PixelPoint const pnt ) const
	{
		return fPixelPoint( convert2fPIXEL(pnt.GetX()), convert2fPIXEL(pnt.GetY()) );
	}

	MicroMeterPoint convert2MicroMeterPoint( PixelPoint const pnt ) const
	{ 
		fPixelPoint const fPnt = convert2fPixelPoint( pnt );
		return convert2MicroMeterPoint( fPnt );
	}

	//////// manipulation functions ////////

	bool SetPixelSize( NanoMeter const nmPixelSize )
	{
		bool bValid = isValidPixelSize( nmPixelSize );
		if ( bValid )
			m_pixelSize = nmPixelSize;
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
	NanoMeter   m_pixelSize;
	SmoothMove  m_smoothMove;
	bool        m_bMoving;
};

PixelPoint NNetPixel2PixelSize( PixelPoint const,   PixelCoordsFp const *, PixelCoordsFp const * );
PixelPoint NNetPixel2PixelPos ( PixelPoint const,   PixelCoordsFp const *, PixelCoordsFp const * );
PixelRect  NNetPixel2PixelRect( PixelRect  const &, PixelCoordsFp const *, PixelCoordsFp const * );
