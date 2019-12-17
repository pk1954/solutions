// Direct2D.cpp
//

#include "stdafx.h"
#include <string.h>
#include "d2d1.h"
#include "d2d1helper.h"
#include "dwrite.h"
#include "util.h"
#include "win32_util.h"
#include "PixelTypes.h"
#include "Direct2D.h"

D2D_driver::D2D_driver( ):
	m_pD2DFactory( nullptr ),
	m_pRenderTarget( nullptr ),
	m_pDWriteFactory( nullptr ),
	m_pStdTextFormat( nullptr ),
	m_hwnd( nullptr ),
	m_hr( 0 )
{
}

D2D_driver::~D2D_driver()
{
	discardResources( );
}

void D2D_driver::createResources( ) 
{
	m_hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		& m_pD2DFactory
	);
	assert( SUCCEEDED( m_hr ) );

	m_hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown **>(& m_pDWriteFactory)
	);
	assert( SUCCEEDED( m_hr ) );

	RECT rc = Util::GetClRect( m_hwnd );

	m_hr = m_pD2DFactory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties( m_hwnd, D2D1::SizeU( rc.right - rc.left, rc.bottom - rc.top )	),
		& m_pRenderTarget
	);
	assert( SUCCEEDED( m_hr ) );

	//	m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
}

void D2D_driver::discardResources( ) 
{
	SafeRelease( & m_pD2DFactory );
	SafeRelease( & m_pDWriteFactory );
	SafeRelease( & m_pRenderTarget );
}

void D2D_driver::Initialize( HWND const hwndApp ) 
{
	m_hwnd = hwndApp;
	createResources( );
}

void D2D_driver::Resize( int const iWidth, int const iHeight )
{
	if ( m_pRenderTarget )
	{
		m_pRenderTarget->Resize( D2D1::SizeU( iWidth, iHeight ) );
	}
}

void D2D_driver::ShutDown( )
{
	discardResources( );
}

void D2D_driver::DeleteTextFormat( IDWriteTextFormat ** ppFormat )
{
	SafeRelease( ppFormat );
}

IDWriteTextFormat * D2D_driver::NewTextFormat( float const fSize )
{
	IDWriteTextFormat * pTextFormat;
	m_hr = m_pDWriteFactory->CreateTextFormat
	(
		L"",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fSize,
		L"", //locale
		& pTextFormat
	);
	assert( SUCCEEDED( m_hr ) );

	m_hr = pTextFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_CENTER );

	return pTextFormat;
}

void D2D_driver::SetStdFontSize( float const fSize )
{
	SafeRelease( & m_pStdTextFormat );
	m_pStdTextFormat = NewTextFormat( fSize );
}

// functions called per frame

bool D2D_driver::StartFrame( HWND const hwnd, HDC const hdc )
{
	if ( ! m_pRenderTarget )
		createResources( );
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::Azure) );
	return true;
}

void D2D_driver::DisplayText
( 
	PixelRect           const & pixRect, 
	std::wstring        const & wstr,
	COLORREF            const   color,
	IDWriteTextFormat * const   pTextFormat
)
{
	IDWriteTextFormat    * pTF { pTextFormat ? pTextFormat : m_pStdTextFormat };
	ID2D1SolidColorBrush * pBrush { createBrush( color ) };
	D2D1_RECT_F            d2Rect 
	{ 
		static_cast<float>(pixRect.GetLeft  ().GetValue()),
		static_cast<float>(pixRect.GetTop   ().GetValue()),
		static_cast<float>(pixRect.GetRight ().GetValue()),
		static_cast<float>(pixRect.GetBottom().GetValue())
	};
	m_pRenderTarget->DrawText( wstr.c_str(), static_cast<UINT32>(wstr.length()), pTF, d2Rect, pBrush );
	SafeRelease( & pBrush );
}

// Finish rendering; page flip.

void D2D_driver::EndFrame( HWND const hwnd )
{
	m_hr = m_pRenderTarget->EndDraw();
	if (m_hr == D2DERR_RECREATE_TARGET)
	{
		m_hr = S_OK;
		discardResources( );
	}
}

void D2D_driver::DrawLine
( 
	fPixelPoint const & fpp1, 
	fPixelPoint const & fpp2, 
	fPIXEL      const   fpixWidth, 
	COLORREF    const   color
)
{
	ID2D1SolidColorBrush * pBrush = createBrush( color );

	m_pRenderTarget->DrawLine
	( 
		D2D1_POINT_2F{ fpp1.GetXvalue(), fpp1.GetYvalue() }, 
		D2D1_POINT_2F{ fpp2.GetXvalue(), fpp2.GetYvalue() },
		pBrush,
		fpixWidth.GetValue()
	);

	SafeRelease( & pBrush );
}

void D2D_driver::DrawPolygon
(
	int         const iNrOfEdges,
	fPixelPoint const ptPos,
	COLORREF    const color, 
	fPIXEL      const fPixRadius 
)
{
	DrawCircle( ptPos, color, fPixRadius );
}

void D2D_driver::DrawCircle
(
	fPixelPoint const ptPos,
	COLORREF    const color, 
	fPIXEL      const fPixRadius 
)
{
	ID2D1SolidColorBrush * pBrush = createBrush( color );
	D2D1_ELLIPSE ellipse { D2D1_POINT_2F{ ptPos.GetXvalue(), ptPos.GetYvalue() }, fPixRadius.GetValue(), fPixRadius.GetValue() }; 
	m_pRenderTarget->FillEllipse( & ellipse, pBrush	);
	SafeRelease( & pBrush );
}

ID2D1SolidColorBrush * D2D_driver::createBrush( COLORREF const color )
{
	D2D1::ColorF d2dCol
	{
		static_cast<float>(GetRValue(color) / 255.0), 
		static_cast<float>(GetGValue(color) / 255.0), 
		static_cast<float>(GetBValue(color) / 255.0), 
		1.0f 
	};

	ID2D1SolidColorBrush * pBrush;
	m_pRenderTarget->CreateSolidColorBrush( d2dCol, & pBrush ); 
	return pBrush;
}
