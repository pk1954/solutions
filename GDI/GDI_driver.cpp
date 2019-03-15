// GDI_driver.cpp
//
// GDI

#include "stdafx.h"
#include <string.h>
#include "util.h"
#include "win32_util.h"
#include "PixelTypes.h"
#include "GDI_driver.h"

GDI_driver::GDI_driver( ) 
{
}

GDI_driver::~GDI_driver()
{
}

PixelRect GDI_driver::CalcGraphicsRect( std::wstring const & wstr )
{
	RECT rect{ 0, 0, 0, 0 };
	return Util::RECT2PixelRect( rect );
}

void GDI_driver::DisplayGraphicsText( PixelRect const & pixRect, std::wstring const & wstr )
{
	RECT rect( Util::PixelRect2RECT( pixRect ) );			  
}

bool GDI_driver::StartFrame( HWND const hwnd, HDC const hDC )
{
	m_hDC = hDC;
	return true;
}

void GDI_driver::SetFontSize( PIXEL const nPointSize )
{
}

void GDI_driver::AddIndividual( PixelPoint const ptPos, COLORREF const color, float const fPixSize )
{
    float const fPtPosx = static_cast<float>( ptPos.GetXvalue() );
    float const fPtPosy = static_cast<float>( ptPos.GetYvalue() );

    PIXEL_X pixStartX { static_cast<PIXEL>(CastToLong(fPtPosx - fPixSize)) };
    PIXEL_Y pixStartY { static_cast<PIXEL>(CastToLong(fPtPosy - fPixSize)) };
    PIXEL_X pixEndX   { static_cast<PIXEL>(CastToLong(fPtPosx + fPixSize)) };
	PIXEL_Y pixEndY   { static_cast<PIXEL>(CastToLong(fPtPosy + fPixSize)) };

	PixelRect const pixRect{ pixStartX, pixStartY, pixEndX, pixEndY };
	RECT      const rect = Util::PixelRect2RECT( pixRect );
	HBRUSH    const hBrush = CreateSolidBrush( color );
	FillRect( m_hDC, & rect, hBrush );
	DeleteObject( hBrush );
}

void GDI_driver::AddBackGround( PixelPoint const ptPos, COLORREF const color, float const fPixSize )
{
	float const fPtPosx = static_cast<float>( ptPos.GetXvalue() );
    float const fPtPosy = static_cast<float>( ptPos.GetYvalue() );

	float const fPixSizeHalf = fPixSize / 2;

}

void GDI_driver::RenderTranspRect
( 
	PixelRect    const & rectTransparent, 
	unsigned int const   uiALpha,
	COLORREF     const   color
)
{
	if ( rectTransparent.IsNotEmpty() )
	{
	}
}

void GDI_driver::RenderBackground( )
{
}

void GDI_driver::RenderIndividuals( )
{
}

void GDI_driver::EndFrame( )
{
}
