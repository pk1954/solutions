// Direct2D.cpp
//

#include "stdafx.h"
#include <string.h>
#include "d2d1.h"
#include "dwrite.h"
#include "util.h"
#include "win32_util.h"
#include "PixelTypes.h"
#include "Direct2D.h"

D2D_driver::D2D_driver( ):
	m_pD2DFactory( nullptr ),
	m_pRenderTarget( nullptr ),
	m_pDWriteFactory( nullptr ),
	m_pTextFormat( nullptr ),
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

	RECT rc { Util::GetClRect( m_hwnd ) };

	m_hr = m_pD2DFactory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties( m_hwnd, D2D1::SizeU( rc.right - rc.left, rc.bottom - rc.top )	),
		& m_pRenderTarget
	);
	assert( SUCCEEDED( m_hr ) );

	m_pTextFormat = NewTextFormat( 12.0f );

	//	m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
}

void D2D_driver::discardResources( ) 
{
	SafeRelease( & m_pD2DFactory );
	SafeRelease( & m_pDWriteFactory );
	SafeRelease( & m_pRenderTarget );
	SafeRelease( & m_pTextFormat );
}

void D2D_driver::Initialize( HWND const hwnd ) 
{
	m_hwnd = hwnd;
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

IDWriteTextFormat * D2D_driver::NewTextFormat( float const fSize ) const
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
	SafeRelease( & m_pTextFormat );
	m_pTextFormat = NewTextFormat( fSize );
}

// functions called per frame

bool D2D_driver::StartFrame( HDC const hdc )
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
	D2D1::ColorF        const   colF,
	IDWriteTextFormat * const   pTextFormat
) const
{
	IDWriteTextFormat    * pTF { pTextFormat ? pTextFormat : m_pTextFormat };
	ID2D1SolidColorBrush * pBrush { createBrush( colF ) };
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

void D2D_driver::EndFrame( )
{
	m_hr = m_pRenderTarget->EndDraw();
	if (m_hr == D2DERR_RECREATE_TARGET)
	{
		m_hr = S_OK;
		discardResources( );
	}
}

void D2D_driver::DrawTranspRect( fPixelRect const & rect, D2D1::ColorF const colF ) const
{
	fPIXEL const MIN_SIZE { 10._fPIXEL };

	ID2D1SolidColorBrush * pBrush { createBrush( colF ) };
	pBrush->SetOpacity( 0.5f );

	if ( (rect.GetHeight( ) > MIN_SIZE) && (rect.GetWidth( ) > MIN_SIZE) )
	{
		m_pRenderTarget->FillRectangle
		( 
			D2D1_RECT_F
			{ 
				rect.GetLeft  ().GetValue(), 
				rect.GetTop   ().GetValue(), 
				rect.GetRight ().GetValue(), 
				rect.GetBottom().GetValue() 
			},
			pBrush 
		);
	}
	else
	{
		fPixelRectSize fClientSize { Convert2fPixelRectSize( Util::GetClRectSize( m_hwnd ) ) };

		m_pRenderTarget->FillRectangle
		( 
			D2D1_RECT_F
			{ 
				0, 
				0, 
				rect.GetRight ().GetValue(), 
				rect.GetBottom().GetValue() 
			},
			pBrush 
		);

		m_pRenderTarget->FillRectangle
		( 
			D2D1_RECT_F
			{ 
				rect.GetLeft().GetValue(), 
				rect.GetTop ().GetValue(), 
				fClientSize.GetXvalue(), 
				fClientSize.GetYvalue() 
			},
			pBrush 
		);
	}

	SafeRelease( & pBrush );
}

void D2D_driver::DrawLine
( 
	fPixelPoint  const & fpp1, 
	fPixelPoint  const & fpp2, 
	fPIXEL       const   fpixWidth, 
	D2D1::ColorF const   colF
) const
{
	ID2D1SolidColorBrush * pBrush { createBrush( colF ) };

	m_pRenderTarget->DrawLine
	( 
		D2D1_POINT_2F{ fpp1.GetXvalue(), fpp1.GetYvalue() }, 
		D2D1_POINT_2F{ fpp2.GetXvalue(), fpp2.GetYvalue() },
		pBrush,
		fpixWidth.GetValue()
	);

	SafeRelease( & pBrush );
}

void D2D_driver::DrawCircle
(
	fPixelCircle const & circle,
	D2D1::ColorF const   colF
) const
{
	ID2D1SolidColorBrush * pBrush { createBrush( colF ) };
	D2D1_ELLIPSE ellipse 
	{ 
		D2D1_POINT_2F{ circle.GetPosition().GetXvalue(), circle.GetPosition().GetYvalue() }, 
		circle.GetRadius().GetValue(), 
		circle.GetRadius().GetValue() 
	}; 
	m_pRenderTarget->FillEllipse( & ellipse, pBrush	);
	SafeRelease( & pBrush );
}

void D2D_driver::DrawArrow
(
	fPixelPoint  const ptPos,
	fPixelPoint  const ptVector,
	fPIXEL       const fPixSize,  
	fPIXEL       const fPixWidth, 
	D2D1::ColorF const colF
	) const
{
	if ( ! IsCloseToZero( ptVector ) )
	{
		fPixelPoint const ptVectorScaled { ptVector * ( fPixSize / Hypot( ptVector ) )  };
		fPixelPoint const ptOrtho        { OrthoVector( ptVectorScaled, fPixSize ) };
		fPixelPoint const ptEndPoint1    { ptPos - ptVectorScaled + ptOrtho };
		fPixelPoint const ptEndPoint2    { ptPos - ptVectorScaled - ptOrtho };

		DrawLine( ptPos, ptEndPoint1, fPixWidth, colF );
		DrawLine( ptPos, ptEndPoint2, fPixWidth, colF );
	}
}

ID2D1SolidColorBrush * D2D_driver::createBrush( D2D1::ColorF const d2dCol ) const
{
	ID2D1SolidColorBrush * pBrush;
	HRESULT hres = m_pRenderTarget->CreateSolidColorBrush( d2dCol, & pBrush ); 
	assert( hres == S_OK );
	return pBrush;
}
