// PixelCoordsFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "SmoothMove.h"

MicroMeter const MINIMUM_PIXEL_SIZE =    0.1_MicroMeter;
MicroMeter const DEFAULT_PIXEL_SIZE =    1.0_MicroMeter;  
MicroMeter const MAXIMUM_PIXEL_SIZE = 2000.0_MicroMeter;  // 2 MilliMeter

class PixelCoordsFp
{
public:

	PixelCoordsFp()
	  : m_fPixOffset( 0.0_fPIXEL ),
		m_pixelSize ( DEFAULT_PIXEL_SIZE ),
		m_bMoving   ( false )
	{}

	//////// transformations ////////

	fPIXEL convert2fPixel( MicroMeter const param ) const
	{ 
		MicroMeter res( param / m_pixelSize.GetValue() );
		return fPIXEL( res.GetValue() );
	}

	MicroMeter convert2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return MicroMeter( m_pixelSize * fPixel.GetValue() );
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

	MicroMeter  GetPixelSize( )    const { return m_pixelSize; };
	
	fPixelPoint GetfPixelOffset( ) const { return m_fPixOffset; }

	MicroMeter ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		MicroMeter newSize { m_pixelSize };
		if ( bZoomIn )
		{
			newSize /= 1.3f;
		}
		else
		{
			newSize *= 1.3f;
		}
		return isValidPixelSize(newSize) ? newSize : m_pixelSize;
	}

	PIXEL convert2PIXEL( fPIXEL const fP ) const
	{
		return PIXEL( CastToLong( fP.GetValue() ) );
	}

	fPIXEL convert2fPIXEL( PIXEL const p ) const
	{
		return fPIXEL( static_cast<float>( p.GetValue() ) );
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

	fPixelPoint calcCenterOffset            // calculate new pixel offset,
	(                                       // which keeps umPntCenter at fPixCenter
		MicroMeterPoint const umPntCenter,  // do not yet set m_fPixOffset to this value!
		fPixelPoint     const fPixCenter 
	)  
	{
//		assert( IsInSimulationArea( umPntCenter ) );
		fPixelPoint const fPixPnt( convert2fPixelSize( umPntCenter ) );
		fPixelPoint const fPixOffset( fPixPnt - fPixCenter );
		return fPixOffset;
	}
	
	//////// manipulation functions ////////

	bool ZoomNNet( MicroMeter const pixelSize )
	{
		bool bValid = isValidPixelSize( pixelSize );
		if ( bValid )
			m_pixelSize = pixelSize;
		return bValid;
	}

	void MoveNNet( PixelPoint const pntDelta )
	{
		auto fPntDelta = fPixelPoint
		( 
			fPIXEL( static_cast<float>(pntDelta.GetXvalue()) ), 
			fPIXEL( static_cast<float>(pntDelta.GetYvalue()) )
		);
		m_fPixOffset -= fPntDelta;
	}

	void CenterSimulationArea
	( 
		MicroMeterPoint const umPntCenter,   
		fPixelPoint     const fPntPix  
	)
	{
		m_fPixOffset = calcCenterOffset( umPntCenter, fPntPix );
	}

private:

	bool isValidPixelSize( MicroMeter const newSize ) const
	{
		return (MINIMUM_PIXEL_SIZE <= newSize) && (newSize <= MAXIMUM_PIXEL_SIZE); 
	}

	fPixelPoint m_fPixOffset;
	MicroMeter  m_pixelSize;
	bool        m_bMoving;
};

PixelPoint NNetPixel2PixelSize( PixelPoint const,   PixelCoordsFp const *, PixelCoordsFp const * );
PixelPoint NNetPixel2PixelPos ( PixelPoint const,   PixelCoordsFp const *, PixelCoordsFp const * );
PixelRect  NNetPixel2PixelRect( PixelRect  const &, PixelCoordsFp const *, PixelCoordsFp const * );
