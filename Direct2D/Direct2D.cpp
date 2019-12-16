// Direct2D.cpp
//

#include "stdafx.h"
#include <string.h>
#include "d2d1.h"
#include "d2d1helper.h"
#include "util.h"
#include "win32_util.h"
#include "PixelTypes.h"
#include "Direct2D.h"

D2D_driver::D2D_driver( ):
	m_pD2DFactory( nullptr ),
	m_pRenderTarget( nullptr ),
	m_pBlackBrush( nullptr ),
	m_hr( 0 )
{
	m_hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		& m_pD2DFactory
	);
	assert( SUCCEEDED( m_hr ) );
}

D2D_driver::~D2D_driver()
{
	SafeRelease( &  m_pD2DFactory );
}

void D2D_driver::Initialize
( 
	HWND  const hwndApp, 
	ULONG const ulModelWidth, 
	ULONG const ulModelHeight, 
	BOOL  const bHexagon 
) 
{
	RECT rc;
	GetClientRect( hwndApp, &rc );

	m_hr = m_pD2DFactory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties
		(
			hwndApp,
			D2D1::SizeU( rc.right - rc.left, rc.bottom - rc.top )
		),
		& m_pRenderTarget
	);
	assert( SUCCEEDED( m_hr ) );

//	m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );

	m_pRenderTarget->CreateSolidColorBrush
	(
		D2D1::ColorF( D2D1::ColorF::Black ),
		& m_pBlackBrush
	); 
	assert( SUCCEEDED( m_hr ) );
}

void D2D_driver::Resize( int const iWidth, int const iHeight )
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize( D2D1::SizeU( iWidth, iHeight ) );
	}
}

void D2D_driver::ShutDown( )
{
	SafeRelease( & m_pRenderTarget );
	SafeRelease( & m_pBlackBrush );
}

PixelRect D2D_driver::CalcGraphicsRect( wstring const & wstr )
{
	RECT rect{ 0, 0, 0, 0 };
	return Util::RECT2PixelRect( rect );
}

void D2D_driver::DisplayGraphicsText
( 
	PixelRect    const & pixRect, 
	std::wstring const & wstr,
	DWORD        const   format,
	COLORREF     const   col 
)
{
}

// functions called per frame

bool D2D_driver::StartFrame( HWND const hwnd, HDC const hdc )
{
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::Azure) );
	return true;
}

void D2D_driver::SetFontSize( PIXEL const nPointSize )
{
}

void D2D_driver::AddBackGround( PixelPoint const ptPos, COLORREF const color, float const fPixSize )
{
}

void D2D_driver::RenderTranspRect
( 
	PixelRect    const & rectTransparent, 
	unsigned int const   uiALpha,
	COLORREF     const   color
)
{
}

void D2D_driver::RenderBackground( )
{
}

void D2D_driver::RenderForegroundObjects( )
{
}

// Finish rendering; page flip.

void D2D_driver::EndFrame( HWND const hwnd )
{
	m_hr = m_pRenderTarget->EndDraw();
	assert( SUCCEEDED( m_hr ) );
}

void D2D_driver::DrawLine
( 
	fPixelPoint const & fpp1, 
	fPixelPoint const & fpp2, 
	fPIXEL      const   fpixWidth, 
	COLORREF    const   color
)
{
	ID2D1SolidColorBrush * m_pBrush;
	D2D1::ColorF d2dCol
	{
		GetRValue(color) / 255.0, 
		GetGValue(color) / 255.0, 
		GetBValue(color) / 255.0, 
		1.f 
	};

	m_pRenderTarget->CreateSolidColorBrush( d2dCol, & m_pBrush ); 

	m_pRenderTarget->DrawLine
	( 
		D2D1_POINT_2F{ fpp1.GetXvalue(), fpp1.GetYvalue() }, 
		D2D1_POINT_2F{ fpp2.GetXvalue(), fpp2.GetYvalue() },
		m_pBrush,
		fpixWidth.GetValue()
	);
}

void D2D_driver::DrawPolygon
(
	int         const iNrOfEdges,
	fPixelPoint const ptPos,
	COLORREF    const color, 
	fPIXEL      const fPixRadius 
)
{
}
