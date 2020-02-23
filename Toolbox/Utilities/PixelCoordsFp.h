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
		m_pixelSize ( DEFAULT_PIXEL_SIZE )
	{}

	//////// transformations MicroMeter <---> fPixel ////////

	fPIXEL convert2fPixel( MicroMeter const param ) const
	{ 
		return fPIXEL( param / m_pixelSize );
	}

	MicroMeter convert2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	fPixelPoint convert2fPixelSize( MicroMeterPoint const np ) const
	{ 
		return fPixelPoint( convert2fPixel( np.GetX() ), convert2fPixel( np.GetY() ) );
	}

	MicroMeterPoint convert2MicroMeterPointSize( fPixelPoint const pp ) const
	{ 
		return MicroMeterPoint( convert2MicroMeter( pp.GetX() ), convert2MicroMeter( pp.GetY() ) ); 
	}

	MicroMeterPoint convert2MicroMeterPointPos( fPixelPoint const pp ) const
	{ 
		return convert2MicroMeterPointSize( pp + m_fPixOffset );
	}

	fPixelPoint convert2fPixelPos( MicroMeterPoint const np ) const
	{ 
		return convert2fPixelSize( np ) - m_fPixOffset;
	}

	MicroMeterRect convert2MicroMeterRect( fPixelRect const fPixRect ) const
	{ 
		return MicroMeterRect
		( 
			convert2MicroMeterPointPos( fPixRect.GetStartPoint() ), 
			convert2MicroMeterPointPos( fPixRect.GetEndPoint  () ) 
		);
	}

	//////// transformations PIXEL <---> fPixel ////////

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

	fPixelRect convert2fPixelRect( PixelRect const rect ) const
	{
		return fPixelRect
		       ( 
				   convert2fPixelPoint( rect.GetStartPoint() ), 
				   convert2fPixelPoint( rect.GetEndPoint  () ) 
			   );
	}

	//////// transformations MicroMeter <---> PIXEL  ////////

	MicroMeter convert2MicroMeter( PIXEL const pix ) const
	{ 
		return convert2MicroMeter( convert2fPIXEL( pix ) );
	}

	MicroMeterPoint convert2MicroMeterPointPos( PixelPoint const pnt ) const
	{ 
		return convert2MicroMeterPointPos( convert2fPixelPoint( pnt ) );
	}

	MicroMeterRect convert2MicroMeterRect( PixelRect const rect ) const
	{ 
		return convert2MicroMeterRect( convert2fPixelRect( rect ) );
	}

	//////// queries ////////

	MicroMeter  GetPixelSize( )    const { return m_pixelSize; };
	
	fPixelPoint GetfPixelOffset( ) const { return m_fPixOffset; }

	MicroMeter ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		static float const ZOOM_FACTOR { 1.3f };

		float      const fFactor { bZoomIn ? 1.0f / ZOOM_FACTOR : ZOOM_FACTOR };
		MicroMeter const newSize { m_pixelSize * fFactor };
		return isValidPixelSize( newSize ) ? newSize : m_pixelSize;
	}

	fPixelPoint CalcCenterOffset            // calculate new pixel offset,
	(                                       // which keeps umPntCenter at fPixCenter
		MicroMeterPoint const umPntCenter,  // do not yet set m_fPixOffset to this value!
		fPixelPoint     const fPixCenter 
	)  
	{
		fPixelPoint const fPixPnt( convert2fPixelSize( umPntCenter ) );
		fPixelPoint const fPixOffset( fPixPnt - fPixCenter );
		return fPixOffset;
	}
	
	//////// manipulation functions ////////

	bool Zoom( MicroMeter const pixelSize )
	{
		bool bValid = isValidPixelSize( pixelSize );
		if ( bValid )
			m_pixelSize = pixelSize;
		return bValid;
	}

	void Move( PixelPoint const pntDelta )
	{
		m_fPixOffset -= convert2fPixelPoint( pntDelta );
	}

	void Center
	( 
		MicroMeterPoint const umPntCenter,   
		fPixelPoint     const fPntPix  
	)
	{
		m_fPixOffset = CalcCenterOffset( umPntCenter, fPntPix );
	}

	bool CenterPoi   // returns TRUE, if POI was already centered, or if no POI defined
	( 
		fPixelPoint fPixOffsetDesired 
	)
	{
		PixelPoint ppActual  { convert2PixelPoint( m_fPixOffset ) };
		PixelPoint ppDesired { convert2PixelPoint( fPixOffsetDesired ) };

		bool bCentered { ppActual == ppDesired };

		if ( ! bCentered )
		{
			m_fPixOffset = convert2fPixelPoint( m_smoothMove.Step( ppActual, ppDesired ) );
		}

		return bCentered;
	}

	bool ZoomPoi   // returns TRUE, if POI was already zoomed as desired
	( 
		MicroMeter  const umPixelSizeDesired, 
		fPixelPoint const fPixPointCenter
	)
	{
		MicroMeterPoint const umPointcenter { convert2MicroMeterPointPos( fPixPointCenter ) };

		if ( IsCloseToZero(m_pixelSize - umPixelSizeDesired) )
			return true;
 
		m_pixelSize = m_smoothMove.Step( m_pixelSize, umPixelSizeDesired );
		Center( umPointcenter, fPixPointCenter ); 
		return false;
	}

	MicroMeter LimitPixelSize( MicroMeter const sizeDesired ) const
	{
		return ClipToMinMax<MicroMeter>( sizeDesired, MINIMUM_PIXEL_SIZE, MAXIMUM_PIXEL_SIZE );
	}

private:

	bool isValidPixelSize( MicroMeter const newSize ) const
	{
		return (MINIMUM_PIXEL_SIZE <= newSize) && (newSize <= MAXIMUM_PIXEL_SIZE); 
	}

	fPixelPoint m_fPixOffset;
	MicroMeter  m_pixelSize;
	SmoothMove  m_smoothMove;
};
