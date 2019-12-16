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
	m_pPathGeometry( nullptr ),
	m_pSink( nullptr ),
	m_hr( 0 ),
	m_fOrtho( fPixelPoint::NULL_VAL() ),
	m_fp1( fPixelPoint::NULL_VAL() ),
	m_fp2( fPixelPoint::NULL_VAL() )
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

void D2D_driver::AddIndividual( PixelPoint const ptPos, COLORREF const color, float const fPixSize )
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
	if ( ! SUCCEEDED( m_hr ) )
	{
		int x = 8765;
	}
}

void D2D_driver::AddfPixelLine
( 
	fPixelPoint const & fpp1, 
	fPixelPoint const & fpp2, 
	fPIXEL      const   fpixWidth, 
	COLORREF    const   color
)
{
}

void D2D_driver::StartPipeline
(
	fPixelPoint const & fppStart, 
	fPixelPoint const & fppEnd, 
	fPIXEL      const   fpixWidth, 
	COLORREF    const   color
)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse( D2D1::Point2F(0.f, 0.f), 750.f, 500.f );
	m_pRenderTarget->DrawEllipse( ellipse, m_pBlackBrush, 10.f );

	D2D1_RECT_F rectangle = D2D1::Rect( 100.f, 100.f, 300.f, 200.f );

	ID2D1RectangleGeometry * pRectangle;
	m_pD2DFactory->CreateRectangleGeometry( rectangle, &pRectangle );
	m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Rotation( 30.f, D2D1_POINT_2F{ 150.f, 150.f } ) );
	m_pRenderTarget->FillRectangle( rectangle, m_pBlackBrush );
	m_pRenderTarget->DrawLine( D2D1_POINT_2F{ 250.f, 250.f }, D2D1_POINT_2F{ 670.f, 350.f }, m_pBlackBrush, 30.f );
	//m_hr = m_pD2DFactory->CreatePathGeometry( & m_pPathGeometry ); 
	//if ( ! SUCCEEDED( m_hr ) )
	//{
	//	int x = 8765;
	//}
	//m_fOrtho = OrthoVector( fppEnd - fppStart, fpixWidth );
	//m_hr = m_pPathGeometry->Open( & m_pSink ); 
	//if ( ! SUCCEEDED( m_hr ) )
	//{
	//	int x = 8765;
	//}
}

void D2D_driver::AddPipelinePoint
(
	fPixelPoint const & fpp, 
	COLORREF    const   color
)
{
	fPixelPoint fp3 = fpp + m_fOrtho;
	fPixelPoint fp4 = fpp - m_fOrtho;

	//m_pSink->BeginFigure( D2D1::Point2F( m_fp1.GetXvalue(), m_fp1.GetYvalue() ), D2D1_FIGURE_BEGIN_FILLED	);
	//m_pSink->AddLine    ( D2D1::Point2F( m_fp2.GetXvalue(), m_fp2.GetYvalue() ));
	//m_pSink->AddLine    ( D2D1::Point2F( fp3  .GetXvalue(), fp3  .GetYvalue() ));
	//m_pSink->AddLine    ( D2D1::Point2F( fp4  .GetXvalue(), fp4  .GetYvalue() ));
	//m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
}

void D2D_driver::RenderPipeline( )
{
	//m_hr = m_pSink->Close();
	//if ( ! SUCCEEDED( m_hr ) )
	//{
	//	int x = 8765;
	//}

	//SafeRelease( & m_pSink );
	//SafeRelease( & m_pPathGeometry );
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
