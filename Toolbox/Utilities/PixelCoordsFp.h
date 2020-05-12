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

	void Reset( )
	{
		m_fPixOffset = 0.0_fPIXEL;
		m_pixelSize  = DEFAULT_PIXEL_SIZE;
	}

	//////// transformations MicroMeter <---> fPixel ////////

	fPIXEL const Convert2fPixel( MicroMeter const param ) const
	{ 
		return fPIXEL( param / m_pixelSize );
	}

	MicroMeter const Convert2MicroMeter( fPIXEL const fPixel ) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	fPixelPoint const Convert2fPixelSize( MicroMeterPoint const np ) const
	{ 
		return fPixelPoint( Convert2fPixel( np.GetX() ), Convert2fPixel( np.GetY() ) );
	}

	MicroMeterPoint const Convert2MicroMeterPointSize( fPixelPoint const pp ) const
	{ 
		return MicroMeterPoint( Convert2MicroMeter( pp.GetX() ), Convert2MicroMeter( pp.GetY() ) ); 
	}

	MicroMeterPoint const Convert2MicroMeterPointPos( fPixelPoint const pp ) const
	{ 
		return Convert2MicroMeterPointSize( pp + m_fPixOffset );
	}

	MicroMeterPoint const Convert2MicroMeterPointPos( PixelPoint const pixPoint ) const
	{ 
		return Convert2MicroMeterPointSize( Convert2fPixelPoint( pixPoint ) + m_fPixOffset );
	}

	fPixelPoint const Convert2fPixelPos( MicroMeterPoint const np ) const
	{ 
		return Convert2fPixelSize( np ) - m_fPixOffset;
	}

	MicroMeterRect const Convert2MicroMeterRect( fPixelRect const & fPixRect ) const
	{ 
		return MicroMeterRect
		( 
			Convert2MicroMeterPointPos( fPixRect.GetStartPoint() ), 
			Convert2MicroMeterPointPos( fPixRect.GetEndPoint  () ) 
		);
	}

	fPixelRect const Convert2fPixelRect( MicroMeterRect const & umRect ) const
	{
		return fPixelRect
		( 
			Convert2fPixelPos( umRect.GetStartPoint() ), 
			Convert2fPixelPos( umRect.GetEndPoint  () ) 
		);
	}

	//////// transformations PIXEL <---> fPixel ////////

	PIXEL const Convert2PIXEL( fPIXEL const fP ) const
	{
		return PIXEL( CastToLong( fP.GetValue() ) );
	}

	fPIXEL const Convert2fPIXEL( PIXEL const p ) const
	{
		return fPIXEL( static_cast<float>( p.GetValue() ) );
	}

	PixelPoint const Convert2PixelPoint( fPixelPoint const fPnt ) const
	{
		return PixelPoint( Convert2PIXEL(fPnt.GetX()), Convert2PIXEL(fPnt.GetY()) );
	}

	fPixelPoint const Convert2fPixelPoint( PixelPoint const pnt ) const
	{
		return fPixelPoint( Convert2fPIXEL(pnt.GetX()), Convert2fPIXEL(pnt.GetY()) );
	}

	PixelRect const Convert2PixelRect( fPixelRect const & fPixRect ) const
	{
		return PixelRect
		       (
				   Convert2PixelPoint( fPixRect.GetStartPoint() ),
				   Convert2PixelPoint( fPixRect.GetEndPoint  () )
			   );
	}

	fPixelRect const Convert2fPixelRect( PixelRect const & rect ) const
	{
		return fPixelRect
		       ( 
				   Convert2fPixelPoint( rect.GetStartPoint() ), 
				   Convert2fPixelPoint( rect.GetEndPoint  () ) 
			   );
	}

	//////// transformations MicroMeter <---> PIXEL  ////////

	MicroMeter Convert2MicroMeter( PIXEL const pix ) const
	{ 
		return Convert2MicroMeter( Convert2fPIXEL( pix ) );
	}

	MicroMeterPoint Convert2MicroMeterPointSize( PixelPoint const & pnt ) const
	{ 
		return Convert2MicroMeterPointSize( Convert2fPixelPoint( pnt ) );
	}

	MicroMeterRect Convert2MicroMeterRect( PixelRect const & rect ) const
	{ 
		return Convert2MicroMeterRect( Convert2fPixelRect( rect ) );
	}

	PIXEL Convert2PIXEL( MicroMeter const um ) const
	{
		return Convert2PIXEL(Convert2fPixel( um ) );
	}

	PixelPoint Convert2PixelPos( MicroMeterPoint const & umPnt ) const
	{
		return Convert2PixelPoint( Convert2fPixelPos( umPnt ) );
	}

	PixelRect Convert2PixelRect( MicroMeterRect const & umRect ) const
	{
		return Convert2PixelRect( Convert2fPixelRect( umRect ) );
	}

	//////// queries ////////

	MicroMeter  GetPixelSize( )   const { return m_pixelSize; };
	
	fPixelPoint GetPixelOffset( ) const { return m_fPixOffset; }

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
		m_fPixOffset -= Convert2fPixelPoint( pntDelta );
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
		m_fPixOffset = Convert2fPixelSize( umPntCenter ) - fPntPix;
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
