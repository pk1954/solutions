// PixelCoordsFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"

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

	fPIXEL const convert2fPixel( MicroMeter const param ) const
	{ 
		return fPIXEL( param / m_pixelSize );
	}

	MicroMeter const convert2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	fPixelPoint const convert2fPixelSize( MicroMeterPoint const np ) const
	{ 
		return fPixelPoint( convert2fPixel( np.GetX() ), convert2fPixel( np.GetY() ) );
	}

	MicroMeterPoint const convert2MicroMeterPointSize( fPixelPoint const pp ) const
	{ 
		return MicroMeterPoint( convert2MicroMeter( pp.GetX() ), convert2MicroMeter( pp.GetY() ) ); 
	}

	MicroMeterPoint const convert2MicroMeterPointPos( fPixelPoint const pp ) const
	{ 
		return convert2MicroMeterPointSize( pp + m_fPixOffset );
	}

	MicroMeterPoint const convert2MicroMeterPointPos( PixelPoint const pixPoint ) const
	{ 
		return convert2MicroMeterPointSize( convert2fPixelPoint( pixPoint ) + m_fPixOffset );
	}

	fPixelPoint const convert2fPixelPos( MicroMeterPoint const np ) const
	{ 
		return convert2fPixelSize( np ) - m_fPixOffset;
	}

	MicroMeterRect const convert2MicroMeterRect( fPixelRect const & fPixRect ) const
	{ 
		return MicroMeterRect
		( 
			convert2MicroMeterPointPos( fPixRect.GetStartPoint() ), 
			convert2MicroMeterPointPos( fPixRect.GetEndPoint  () ) 
		);
	}

	fPixelRect const convert2fPixelRect( MicroMeterRect const & umRect ) const
	{
		return fPixelRect
		( 
			convert2fPixelPos( umRect.GetStartPoint() ), 
			convert2fPixelPos( umRect.GetEndPoint  () ) 
		);
	}

	//////// transformations PIXEL <---> fPixel ////////

	PIXEL const convert2PIXEL( fPIXEL const fP ) const
	{
		return PIXEL( CastToLong( fP.GetValue() ) );
	}

	fPIXEL const convert2fPIXEL( PIXEL const p ) const
	{
		return fPIXEL( static_cast<float>( p.GetValue() ) );
	}

	PixelPoint const convert2PixelPoint( fPixelPoint const fPnt ) const
	{
		return PixelPoint( convert2PIXEL(fPnt.GetX()), convert2PIXEL(fPnt.GetY()) );
	}

	fPixelPoint const convert2fPixelPoint( PixelPoint const pnt ) const
	{
		return fPixelPoint( convert2fPIXEL(pnt.GetX()), convert2fPIXEL(pnt.GetY()) );
	}

	PixelRect const convert2PixelRect( fPixelRect const & fPixRect ) const
	{
		return PixelRect
		       (
				   convert2PixelPoint( fPixRect.GetStartPoint() ),
				   convert2PixelPoint( fPixRect.GetEndPoint  () )
			   );
	}

	fPixelRect const convert2fPixelRect( PixelRect const & rect ) const
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

	MicroMeterPoint convert2MicroMeterPointSize( PixelPoint const & pnt ) const
	{ 
		return convert2MicroMeterPointSize( convert2fPixelPoint( pnt ) );
	}

	MicroMeterRect convert2MicroMeterRect( PixelRect const & rect ) const
	{ 
		return convert2MicroMeterRect( convert2fPixelRect( rect ) );
	}

	PIXEL convert2PIXEL( MicroMeter const um ) const
	{
		return PIXEL( CastToLong( um.GetValue() ) );
	}

	PixelPoint convert2PixelPoint( MicroMeterPoint const & umPnt ) const
	{
		return PixelPoint( convert2PIXEL(umPnt.GetX()), convert2PIXEL(umPnt.GetY()) );
	}

	PixelRect convert2PixelRect( MicroMeterRect const & umRect ) const
	{
		return PixelRect
		(
			convert2PixelPoint( umRect.GetStartPoint() ),
			convert2PixelPoint( umRect.GetEndPoint  () )
		);
	}

	//////// queries ////////

	MicroMeter  GetPixelSize( )   const { return m_pixelSize; };
	
	fPixelPoint GetPixelOffset( ) const 
	{ 
		return m_fPixOffset; 
	}

	MicroMeter ComputeNewPixelSize( bool const bZoomIn ) const  // does not modify field size
	{
		static float const ZOOM_FACTOR { 1.3f };

		float      const fFactor { bZoomIn ? 1.0f / ZOOM_FACTOR : ZOOM_FACTOR };
		MicroMeter const newSize { m_pixelSize * fFactor };
		return isValidPixelSize( newSize ) ? newSize : m_pixelSize;
	}

	//////// manipulation functions ////////

	bool Zoom( MicroMeter const pixelSize )
	{
		bool bValid = isValidPixelSize( pixelSize );
		if ( bValid )
			SetPixelSize( pixelSize );
		return bValid;
	}

	void SetPixelSize( MicroMeter const pixelSize )
	{
		m_pixelSize = pixelSize;
	}

	void Move( PixelPoint const pntDelta )
	{
		m_fPixOffset -= convert2fPixelPoint( pntDelta );
	}

	void SetPixelOffset( fPixelPoint const fPixOffset )
	{
		m_fPixOffset = fPixOffset;
	}

	void Center
	( 
		MicroMeterPoint const umPntCenter,   
		fPixelPoint     const fPntPix  
	)
	{
		m_fPixOffset = convert2fPixelSize( umPntCenter ) - fPntPix;
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
};
