// Direct2D.cpp
//
// Toolbox\Direct2D

module;

#include <array>
#include <string.h>
#include "d2d1.h"
#include "dwrite.h"
#include "win32_util.h"
#include "PixelTypes.h"

module Direct2D;

import Util;

using std::array;
using std::wstring;
using std::bit_cast;

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
		bit_cast<IUnknown **>(& m_pDWriteFactory)
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
}

void D2D_driver::discardResources() 
{
	SafeRelease(& m_pD2DFactory);
	SafeRelease(& m_pDWriteFactory);
	SafeRelease(& m_pRenderTarget);
	SafeRelease(& m_pTextFormat);
}

void D2D_driver::InitWindow(HWND const hwnd) 
{
	m_hwnd = hwnd;
	createResources();
}

void D2D_driver::Resize(PIXEL const width, PIXEL const height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width.GetValue(), height.GetValue()));
	}
}

fPixelRectSize D2D_driver::GetClRectSize() const 
{ 
	return Convert2fPixelRectSize(Util::GetClRectSize(m_hwnd));
}

fPixel D2D_driver::GetClRectWidth() const
{
	return Convert2fPixel(Util::GetClientWindowWidth(m_hwnd));
}

fPixel D2D_driver::GetClRectHeight() const
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
		nullptr,
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

bool D2D_driver::StartFrame()
{
	if (! m_pRenderTarget)
		createResources();
	m_pRenderTarget->BeginDraw();
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
	IDWriteTextFormat    * pTF    { pTextFormat ? pTextFormat : m_pTextFormat };
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
	D2D1_RECT_F            d2Rect {	convertD2D(rect) };
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
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
	m_pRenderTarget->DrawRectangle
	(
		convertD2D(rect),
		pBrush,
		fPixWidth.GetValue(),
		nullptr  // solid stroke
	);
	SafeRelease(& pBrush);
}

void D2D_driver::FillRectangle(fPixelRect const & rect, D2D1::ColorF const colF) const
{
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
	m_pRenderTarget->FillRectangle(convertD2D(rect), pBrush);
	SafeRelease(& pBrush);
}

ID2D1GradientStopCollection * D2D_driver::simpleGradientStopCollection
(
	D2D1::ColorF const colF1,
	D2D1::ColorF const colF2 
) const
{
	HRESULT hr;
	ID2D1GradientStopCollection * pGradientStopColl = nullptr;
	array<D2D1_GRADIENT_STOP,2>   gradientStops { D2D1_GRADIENT_STOP(0.0f, colF1), D2D1_GRADIENT_STOP(1.0f, colF2) };
	hr = m_pRenderTarget->CreateGradientStopCollection(gradientStops.data(), 2, &pGradientStopColl);
	assert(SUCCEEDED(hr));
	return pGradientStopColl;
}

void D2D_driver::FillGradientRect
(
	fPixelRect   const & rect, 
	D2D1::ColorF const   colF1,
	D2D1::ColorF const   colF2 
) const
{
	ID2D1GradientStopCollection * pGradientStopColl { simpleGradientStopCollection(colF1, colF2) };
	ID2D1LinearGradientBrush    * m_pLinearGradientBrush;
	HRESULT hr = m_pRenderTarget->CreateLinearGradientBrush
	(
		D2D1::LinearGradientBrushProperties
		(D2D1::Point2F(rect.GetLeft().GetValue(), 0), D2D1::Point2F(rect.GetRight().GetValue(), 0)),
		pGradientStopColl,
		&m_pLinearGradientBrush
	);
	assert(SUCCEEDED(hr));

	D2D1_RECT_F const d2dRect { convertD2D(rect) };
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
	ID2D1GradientStopCollection * pGradientStopColl { simpleGradientStopCollection(colF1, colF2) };
	ID2D1RadialGradientBrush    * m_pRadialGradientBrush;
	HRESULT hr = m_pRenderTarget->CreateRadialGradientBrush
	(
		D2D1::RadialGradientBrushProperties
		(
			convertD2D(fPE.GetPos()),
			convertD2D(fPP_ZERO),
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
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
	DrawLine(fpp1, fpp2, fpixWidth, pBrush);
	SafeRelease(& pBrush);
}

void D2D_driver::DrawLine
(
	fPixelPoint    const & fpp1, 
	fPixelPoint    const & fpp2, 
	fPixel         const   fpixWidth, 
	ID2D1SolidColorBrush * pBrush
) const
{
	m_pRenderTarget->DrawLine(convertD2D(fpp1), convertD2D(fpp2), pBrush, fpixWidth.GetValue());
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
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
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
	ID2D1SolidColorBrush * pBrush { CreateBrush(colF) };
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

void D2D_driver::UpDownArrow
(
	bool         const   bUpArrow, 
	fPixelRect   const & fPixRect, 
	D2D1::ColorF const   color
) const 
{ 
	fPixel       const fPixHorCenter { fPixRect.GetWidth() / 2 };
	fPixel       const fPixHeight    { fPixRect.GetHeight() };
	fPixel       const fPixArrWidth  { fPixHeight * 0.1f };
	float        const fHorDist      { 0.2f };
	fPixelPoint  const fPixStartPnt  { fPixHorCenter, fPixHeight * fHorDist };
	fPixelPoint  const fPixEndPnt    { fPixHorCenter, fPixHeight * (1.0f - fHorDist) };

	DrawLine
	(
		fPixStartPnt,
		fPixelPoint(fPixHorCenter, fPixHeight * (1.0f - fHorDist)),
		fPixHeight * 0.1f,
		color
	);
	FillArrow
	(
		bUpArrow ? fPixStartPnt : fPixEndPnt,
		fPixelPoint(0.0_fPixel, bUpArrow ? -1.0_fPixel : 1.0_fPixel),
		fPixHeight * 0.2f,
		fPixArrWidth,
		color
	);
};

void D2D_driver::FillDiamond
(
	fPixelPoint  const ptPos,
	fPixel       const fPixSize,  
	D2D1::ColorF const colF
) const
{
	fPixelPoint const ptOffset(fPixSize, fPixSize);
	fPixelPoint const ptStart(ptPos - ptOffset);
	fPixelPoint const ptEnd  (ptPos + ptOffset);
	DrawLine(ptStart, ptEnd, fPixSize * sqrtf(8.0f), colF);
}

void D2D_driver::FillBackground(D2D1::ColorF const d2dCol) const
{
	m_pRenderTarget->Clear(d2dCol);
}

ID2D1SolidColorBrush * D2D_driver::CreateBrush(D2D1::ColorF const d2dCol) const
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

D2D1_RECT_F convertD2D(fPixelRect const & rect)
{
	return D2D1_RECT_F
	{ 
		rect.GetLeft  ().GetValue(), 
		rect.GetTop   ().GetValue(), 
		rect.GetRight ().GetValue(), 
		rect.GetBottom().GetValue() 
	};
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
