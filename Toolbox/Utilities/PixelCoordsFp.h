// PixelCoordsFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"

class PixelCoordsFp
{
public:

	PixelCoordsFp()
	  : m_fPixOffset( 0.0_fPixel ),
		m_pixelSize ( DEFAULT_PIXEL_SIZE )
	{}

	void Reset( )
	{
		m_fPixOffset = 0.0_fPixel;
		m_pixelSize  = DEFAULT_PIXEL_SIZE;
	}

	//////// transformations MicroMeter <---> fPixel ////////

	fPixel const Transform2fPixel( MicroMeter const param ) const
	{ 
		return fPixel( param / m_pixelSize );
	}

	MicroMeter const Transform2MicroMeter( fPixel const fPixel ) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	fPixelPoint const Transform2fPixelSize( MicroMeterPoint const np ) const
	{ 
		return fPixelPoint( Transform2fPixel( np.GetX() ), Transform2fPixel( np.GetY() ) );
	}

	fPixelRectSize const Transform2fPixelRectSize( MicroMeterRectSize const np ) const
	{ 
		return fPixelRectSize( Transform2fPixel( np.GetX() ), Transform2fPixel( np.GetY() ) );
	}

	MicroMeterPoint const Transform2MicroMeterPointSize( fPixelPoint const pp ) const
	{ 
		return MicroMeterPoint( Transform2MicroMeter( pp.GetX() ), Transform2MicroMeter( pp.GetY() ) ); 
	}

	MicroMeterRectSize const Transform2MicroMeterRectSize( fPixelRectSize const pp ) const
	{ 
		return MicroMeterRectSize( Transform2MicroMeter( pp.GetX() ), Transform2MicroMeter( pp.GetY() ) ); 
	}

	MicroMeterPoint const Transform2MicroMeterPointPos( fPixelPoint const pp ) const
	{ 
		return Transform2MicroMeterPointSize( pp + m_fPixOffset );
	}

	MicroMeterPoint const Transform2MicroMeterPointPos( PixelPoint const pixPoint ) const
	{ 
		return Transform2MicroMeterPointSize( ::Convert2fPixelPoint( pixPoint ) + m_fPixOffset );
	}

	fPixelPoint const Transform2fPixelPos( MicroMeterPoint const np ) const
	{ 
		return Transform2fPixelSize( np ) - m_fPixOffset;
	}

	MicroMeterRect const Transform2MicroMeterRect( fPixelRect const & fPixRect ) const
	{ 
		return MicroMeterRect
		( 
			Transform2MicroMeterPointPos( fPixRect.GetStartPoint() ), 
			Transform2MicroMeterPointPos( fPixRect.GetEndPoint  () ) 
		);
	}

	fPixelRect const Transform2fPixelRect( MicroMeterRect const & umRect ) const
	{
		return fPixelRect
		( 
			Transform2fPixelPos( umRect.GetStartPoint() ), 
			Transform2fPixelPos( umRect.GetEndPoint  () ) 
		);
	}

	MicroMeterCircle const Transform2MicroMeterCircle( fPixelCircle const & fPixCircle ) const
	{
		return MicroMeterCircle
		( 
			Transform2MicroMeterPointPos( fPixCircle.GetPosition() ),
			Transform2MicroMeter        ( fPixCircle.GetRadius  () )
		);
	}

	fPixelCircle const Transform2fPixelCircle( MicroMeterCircle const & umCircle ) const
	{
		return fPixelCircle
		( 
			Transform2fPixelPos( umCircle.GetPosition() ),
			Transform2fPixel   ( umCircle.GetRadius() )
		);
	}

	fPixelEllipse const Transform2fPixelEllipse( MicroMeterEllipse const & umEllipse ) const
	{
		return fPixelEllipse
		( 
			Transform2fPixelPos( umEllipse.GetPosition() ),
			Transform2fPixel   ( umEllipse.GetRadiusX() ),
			Transform2fPixel   ( umEllipse.GetRadiusY() )
		);
	}

	//////// transformations PIXEL <---> fPixel ////////

	PixelRect const Transform2PixelRect( fPixelRect const & fPixRect ) const
	{
		return PixelRect
		(
			::Convert2PixelPoint( fPixRect.GetStartPoint() ),
			::Convert2PixelPoint( fPixRect.GetEndPoint  () )
		);
	}

	fPixelRect const Transform2fPixelRect( PixelRect const & rect ) const
	{
		return fPixelRect
		( 
			::Convert2fPixelPoint( rect.GetStartPoint() ), 
			::Convert2fPixelPoint( rect.GetEndPoint  () ) 
		);
	}

	fPixelRectSize const Transform2fPixelRectSize( PixelRectSize const & siz ) const
	{
		return fPixelRectSize
		( 
			::Convert2fPixel( siz.GetX() ), 
			::Convert2fPixel( siz.GetY() ) 
		);
	}

	//////// transformations MicroMeter <---> PIXEL  ////////

	MicroMeter Transform2MicroMeter( PIXEL const pix ) const
	{ 
		return Transform2MicroMeter( ::Convert2fPixel( pix ) );
	}

	MicroMeterPoint Transform2MicroMeterPointSize( PixelPoint const & pnt ) const
	{ 
		return Transform2MicroMeterPointSize( ::Convert2fPixelPoint( pnt ) );
	}

	MicroMeterRectSize Transform2MicroMeterRectSize( PixelRectSize const & siz ) const
	{ 
		return Transform2MicroMeterRectSize( Transform2fPixelRectSize( siz ) );
	}

	MicroMeterRect Transform2MicroMeterRect( PixelRect const & rect ) const
	{ 
		return Transform2MicroMeterRect( Transform2fPixelRect( rect ) );
	}

	PIXEL Transform2PIXEL( MicroMeter const um ) const
	{
		return ::Convert2PIXEL(Transform2fPixel( um ) );
	}

	PixelPoint Transform2PixelSize( MicroMeterPoint const & umPnt ) const
	{
		return ::Convert2PixelPoint( Transform2fPixelSize( umPnt ) );
	}

	PixelPoint Transform2PixelPos( MicroMeterPoint const & umPnt ) const
	{
		return ::Convert2PixelPoint( Transform2fPixelPos( umPnt ) );
	}

	PixelRect Transform2PixelRect( MicroMeterRect const & umRect ) const
	{
		return Transform2PixelRect( Transform2fPixelRect( umRect ) );
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

	void SetPixelSize( MicroMeter const pixelSize )
	{
		m_pixelSize = pixelSize;
	}

	void SetPixelOffset( fPixelPoint const fPixOffset )
	{
		m_fPixOffset = fPixOffset;
	}

	void Move( PixelPoint const pntDelta )
	{
		m_fPixOffset -= ::Convert2fPixelPoint( pntDelta );
	}

	void Move( MicroMeterPoint const umDelta )
	{
		m_fPixOffset -= Transform2fPixelSize( umDelta );
	}

	bool Zoom( MicroMeter const pixelSize )
	{
		bool bValid = isValidPixelSize( pixelSize );
		if ( bValid )
			SetPixelSize( pixelSize );
		return bValid;
	}

	void Center
	( 
		MicroMeterPoint const umPntCenter,   
		fPixelPoint     const fPntPix  
	)
	{
		SetPixelOffset( Transform2fPixelSize( umPntCenter ) - fPntPix );
	}

	MicroMeter const ComputeZoom
	(
		MicroMeterRect const & umRect,     
		PixelRectSize  const   pixRectSize,
		float          const   fRatioFactor
	) const
	{
		MicroMeterRectSize const umRectSize       { Transform2MicroMeterRectSize( pixRectSize ) };
		float              const fVerticalRatio   { umRect.GetHeight() / umRectSize.GetY() };
		float              const fHorizontalRatio { umRect.GetWidth () / umRectSize.GetX() };
		float              const fMaxRatio        { max( fVerticalRatio, fHorizontalRatio ) };
		float              const fDesiredRatio    { fMaxRatio * fRatioFactor };
		MicroMeter         const sizeDesired      { GetPixelSize() * fDesiredRatio };
		return ClipToMinMax<MicroMeter>( sizeDesired, MINIMUM_PIXEL_SIZE, MAXIMUM_PIXEL_SIZE );
	}

private:

	inline static MicroMeter const MINIMUM_PIXEL_SIZE {    0.1_MicroMeter };
	inline static MicroMeter const DEFAULT_PIXEL_SIZE {    1.0_MicroMeter };  
	inline static MicroMeter const MAXIMUM_PIXEL_SIZE { 2000.0_MicroMeter };  // 2 MilliMeter

	bool isValidPixelSize( MicroMeter const newSize ) const
	{
		return (MINIMUM_PIXEL_SIZE <= newSize) && (newSize <= MAXIMUM_PIXEL_SIZE); 
	}

	fPixelPoint m_fPixOffset;
	MicroMeter  m_pixelSize;
};
