// pixelCoordinates.h : 
//
// NNetModel

#pragma once

#include "util.h"
#include "PixelTypes.h"
#include "NNetPoint.h"
#include "SmoothMove.h"

NanoMeter const MINIMUM_PIXEL_SIZE =     100.0_NanoMeter;
NanoMeter const DEFAULT_PIXEL_SIZE =    2000.0_NanoMeter;  
NanoMeter const MAXIMUM_PIXEL_SIZE = 2000000.0_NanoMeter;  // 2 Meter

class NNetPixelCoords
{
public:

	NNetPixelCoords()
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

	fPIXEL MicroMeter2fPixel( MicroMeter const param ) const
	{ 
		MicroMeter res( ( param * 1000.0 ) / m_pixelSize.GetValue() );
		return fPIXEL( res.GetValue() );
	}

	MicroMeter fPixel2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return MicroMeter( fPixel.GetValue() * m_pixelSize.GetValue() / 1000.0 );
	}

	fPixelPoint NNet2fPixelSize( NNetPoint const np ) const
	{ 
		return fPixelPoint( MicroMeter2fPixel( np.GetX() ), MicroMeter2fPixel( np.GetY() ) );
	}

	NNetPoint fPixel2NNetPos( fPixelPoint const pp ) const
	{ 
		auto fPixPoint = fPixelPoint( pp + m_fPixOffset );

		auto np = NNetPoint
		( 
			fPixel2MicroMeter( fPixPoint.GetX() ), 
			fPixel2MicroMeter( fPixPoint.GetY() )
		); 

		return np;
	}

	fPixelPoint NNet2fPixelPos( NNetPoint const np ) const
	{ 
		return NNet2fPixelSize( np ) - m_fPixOffset;
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

	//////// manipulation functions ////////

	//void newPixelSize
	//(
	//	NanoMeter   const newPixelSize,
	//	fPixelPoint const fPixPntNewCenter
	//)
	//{

	//}

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

PixelPoint NNetPixel2PixelSize( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelPoint NNetPixel2PixelPos ( PixelPoint const,   NNetPixelCoords const *, NNetPixelCoords const * );
PixelRect  NNetPixel2PixelRect( PixelRect  const &, NNetPixelCoords const *, NNetPixelCoords const * );
