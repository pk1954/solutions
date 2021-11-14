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

using std::wstring;

D2D_driver::D2D_driver():
	m_pD2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr),
	m_hwnd(nullptr),
	m_hr(0)
{
}

D2D_driver::~D2D_driver()
{
	discardResources();
}

void D2D_driver::createResources() 
{
	m_hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		& m_pD2DFactory
	);
	assert(SUCCEEDED(m_hr));

	m_hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown **>(& m_pDWriteFactory)
	);
	assert(SUCCEEDED(m_hr));

	RECT rc { Util::GetClRect(m_hwnd) };

	m_hr = m_pD2DFactory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)	),
		& m_pRenderTarget
	);
	assert(SUCCEEDED(m_hr));

	m_pTextFormat = NewTextFormat(12.0f);

	//	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2D_driver::discardResources() 
{
	SafeRelease(& m_pD2DFactory);
	SafeRelease(& m_pDWriteFactory);
	SafeRelease(& m_pRenderTarget);
	SafeRelease(& m_pTextFormat);
}

void D2D_driver::Initialize(HWND const hwnd) 
{
	m_hwnd = hwnd;
	createResources();
}

void D2D_driver::Resize(int const iWidth, int const iHeight)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(iWidth, iHeight));
	}
}

fPixelRectSize const D2D_driver::GetClRectSize() const 
{ 
	return Convert2fPixelRectSize(Util::GetClRectSize(m_hwnd));
}

fPixel const D2D_driver::GetClRectWidth() const
{
	return Convert2fPixel(Util::GetClientWindowWidth(m_hwnd));
}

fPixel const D2D_driver::GetClRectHeight() const
{
	return Convert2fPixel(Util::GetClientWindowHeight(m_hwnd));
}

void D2D_driver::ShutDown()
{
	discardResources();
}

IDWriteTextFormat * D2D_driver::NewTextFormat(float const fSize) const
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
	assert(SUCCEEDED(m_hr));

	m_hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	return pTextFormat;
}

void D2D_driver::SetStdFontSize(float const fSize)
{
	SafeRelease(& m_pTextFormat);
	m_pTextFormat = NewTextFormat(fSize);
}

// functions called per frame

bool D2D_driver::StartFrame(HDC const hdc)
{
	if (! m_pRenderTarget)
		createResources();
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Azure));
	return true;
}

void D2D_driver::DisplayText
(
	fPixelRect          const & rect, 
	wstring             const & wstr,
	D2D1::ColorF        const   colF,
	IDWriteTextFormat * const   pTextFormat
) const
{
	IDWriteTextFormat    * pTF { pTextFormat ? pTextFormat : m_pTextFormat };
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };
	D2D1_RECT_F            d2Rect 
	{ 
		rect.GetLeft  ().GetValue(), 
		rect.GetTop   ().GetValue(), 
		rect.GetRight ().GetValue(), 
		rect.GetBottom().GetValue() 
	};
	m_pRenderTarget->DrawText(wstr.c_str(), static_cast<UINT32>(wstr.length()), pTF, d2Rect, pBrush);
	SafeRelease(& pBrush);
}

// Finish rendering; page flip.

void D2D_driver::EndFrame()
{
	m_hr = m_pRenderTarget->EndDraw();
	if (m_hr == D2DERR_RECREATE_TARGET)
	{
		m_hr = S_OK;
		discardResources();
	}
}

void D2D_driver::DrawRectangle(fPixelRect const& rect, D2D1::ColorF const colF, fPixel const fPixWidth) const
{
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };
	m_pRenderTarget->DrawRectangle
	(
		D2D1_RECT_F
		{ 
			rect.GetLeft  ().GetValue(), 
			rect.GetTop   ().GetValue(), 
			rect.GetRight ().GetValue(), 
			rect.GetBottom().GetValue() 
		},
		pBrush,
		fPixWidth.GetValue(),
		nullptr  // solid stroke
	);
	SafeRelease(& pBrush);
}

void D2D_driver::FillRectangle(fPixelRect const & rect, D2D1::ColorF const colF) const
{
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };
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
	SafeRelease(& pBrush);
}

void D2D_driver::FillGradientRect
(
	fPixelRect   const & rect, 
	D2D1::ColorF const   colF1,
	D2D1::ColorF const   colF2 
) const
{
	HRESULT hr;
	ID2D1GradientStopCollection * pGradientStopColl = NULL;
	D2D1_GRADIENT_STOP            gradientStops[2] { { 0.0f, colF1 }, { 1.0f, colF2 } };
	hr = m_pRenderTarget->CreateGradientStopCollection(	gradientStops, 2, & pGradientStopColl);
	assert(SUCCEEDED(hr));

	ID2D1LinearGradientBrush * m_pLinearGradientBrush;
	hr = m_pRenderTarget->CreateLinearGradientBrush
	(
		D2D1::LinearGradientBrushProperties
		(D2D1::Point2F(rect.GetLeft().GetValue(), 0), D2D1::Point2F(rect.GetRight().GetValue(), 0)),
		pGradientStopColl,
		&m_pLinearGradientBrush
	);
	assert(SUCCEEDED(hr));

	D2D1_RECT_F const d2dRect 
	{ 
		rect.GetLeft  ().GetValue(), 
		rect.GetTop   ().GetValue(), 
		rect.GetRight ().GetValue(), 
		rect.GetBottom().GetValue() 
	};
	m_pRenderTarget->FillRectangle(& d2dRect, m_pLinearGradientBrush);

	SafeRelease(& m_pLinearGradientBrush);
	SafeRelease(& pGradientStopColl);
}

void D2D_driver::FillGradientEllipse
(
	fPixelEllipse const & fPE, 
	D2D1::ColorF  const   colF1,
	D2D1::ColorF  const   colF2 
) const
{
	HRESULT hr;
	ID2D1GradientStopCollection * pGradientStopColl = NULL;
	D2D1_GRADIENT_STOP            gradientStops[2] { { 0.0f, colF1 }, { 1.0f, colF2 } };
	hr = m_pRenderTarget->CreateGradientStopCollection(	gradientStops, 2, & pGradientStopColl);
	assert(SUCCEEDED(hr));

	ID2D1RadialGradientBrush * m_pRadialGradientBrush;
	hr = m_pRenderTarget->CreateRadialGradientBrush
	(
		D2D1::RadialGradientBrushProperties
		(
			D2D1::Point2F(fPE.GetPos().GetXvalue(), fPE.GetPos().GetYvalue()), 
			D2D1::Point2F(0, 0), 
			fPE.GetRadiusX().GetValue(), 
			fPE.GetRadiusY().GetValue() 
		),

		pGradientStopColl,
		&m_pRadialGradientBrush
	);
	assert(SUCCEEDED(hr));

	m_pRenderTarget->FillEllipse(convertD2D(fPE), m_pRadialGradientBrush);

	SafeRelease(& m_pRadialGradientBrush);
	SafeRelease(& pGradientStopColl);
}

void D2D_driver::FillGradientCircle
(
	fPixelCircle const & circle, 
	D2D1::ColorF const   colF1,
	D2D1::ColorF const   colF2 
) const
{
	FillGradientEllipse(fPixelEllipse { circle }, colF1, colF2);
}

void D2D_driver::DrawLine
(
	fPixelPoint  const & fpp1, 
	fPixelPoint  const & fpp2, 
	fPixel       const   fpixWidth, 
	D2D1::ColorF const   colF
) const
{
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };

	m_pRenderTarget->DrawLine
	(
		D2D1_POINT_2F{ fpp1.GetXvalue(), fpp1.GetYvalue() }, 
		D2D1_POINT_2F{ fpp2.GetXvalue(), fpp2.GetYvalue() },
		pBrush,
		fpixWidth.GetValue()
	);

	SafeRelease(& pBrush);
}

void D2D_driver::FillCircle
(
	fPixelCircle const & circle,
	D2D1::ColorF const   colF
) const
{
	FillEllipse(fPixelEllipse { circle }, colF);
}

void D2D_driver::DrawCircle
(
	fPixelCircle const & circle,
	D2D1::ColorF const   colF,
	fPixel       const   fPixWidth
) const
{
	DrawEllipse(fPixelEllipse { circle }, colF, fPixWidth);
}

void D2D_driver::FillEllipse
(
	fPixelEllipse const & fPE,
	D2D1::ColorF  const   colF
) const
{
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };
	m_pRenderTarget->FillEllipse(convertD2D(fPE), pBrush	);
	SafeRelease(& pBrush);
}

void D2D_driver::DrawEllipse
(
	fPixelEllipse const & fPE,
	D2D1::ColorF  const   colF,
	fPixel        const   fPixWidth
) const
{
	ID2D1SolidColorBrush * pBrush { createBrush(colF) };
	m_pRenderTarget->DrawEllipse(convertD2D(fPE), pBrush, fPixWidth.GetValue(), nullptr);
	SafeRelease(& pBrush);
}

void D2D_driver::FillArrow
(
	fPixelPoint  const ptPos,
	fPixelPoint  const ptVector,
	fPixel       const fPixSize,  
	fPixel       const fPixWidth, 
	D2D1::ColorF const colF
	) const
{
	if (! ptVector.IsCloseToZero())
	{
		fPixelPoint const ptVectorScaled { ptVector.ScaledTo(fPixSize) };
		fPixelPoint const ptOrtho        { ptVectorScaled.OrthoVector() };
		fPixelPoint const ptEndPoint1    { ptPos - ptVectorScaled + ptOrtho };
		fPixelPoint const ptEndPoint2    { ptPos - ptVectorScaled - ptOrtho };

		DrawLine(ptPos, ptEndPoint1, fPixWidth, colF);
		DrawLine(ptPos, ptEndPoint2, fPixWidth, colF);
	}
}

void D2D_driver::FillDiamond
(
	fPixelPoint  const ptPos,
	fPixel       const fPixSize,  
	D2D1::ColorF const colF
) const
{
	fPixelPoint const ptOffset(fPixSize, fPixSize);
	DrawLine(ptPos - ptOffset, ptPos + ptOffset, fPixSize * sqrtf(8.0f), colF);
}

void D2D_driver::FillBackground(D2D1::ColorF const d2dCol) const
{
	FillRectangle(Convert2fPixelRect(Util::GetClPixelRect(m_hwnd)), d2dCol);
}

ID2D1SolidColorBrush * D2D_driver::createBrush(D2D1::ColorF const d2dCol) const
{
	ID2D1SolidColorBrush * pBrush;
	HRESULT hres = m_pRenderTarget->CreateSolidColorBrush(d2dCol, & pBrush); 
	assert(hres == S_OK);
	return pBrush;
}

D2D1_POINT_2F convertD2D(fPixelPoint const & fPP)
{
	return D2D1_POINT_2F{ fPP.GetXvalue(), fPP.GetYvalue() }; 
}

D2D1_ELLIPSE convertD2D(fPixelEllipse const & fPE)
{
	return D2D1_ELLIPSE
	{ 
		convertD2D(fPE.GetPos()), 
		fPE.GetRadiusX().GetValue(), 
		fPE.GetRadiusY().GetValue() 
	}; 
}
