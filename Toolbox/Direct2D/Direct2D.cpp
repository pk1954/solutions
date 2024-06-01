// Direct2D.cpp
//
// Toolbox\Direct2D

module;

#include <bit>
#include <functional>
#include <memory>
#include <array>
#include <string>
#include <cassert>
#include "d2d1.h"
#include "dwrite.h"
#include <Windows.h>

module Direct2D;

import Color;
import Util;
import Win32_PIXEL;

using std::array;
using std::wstring;
using std::bit_cast;
using std::unique_ptr;
using std::make_unique;
using std::function;

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

	RECT rc { ::GetClRect(m_hwnd) };

	m_hr = m_pD2DFactory->CreateHwndRenderTarget
	(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		& m_pRenderTarget
	);
	assert(SUCCEEDED(m_hr));

	m_pTextFormat = NewTextFormat(12.0f);

	static Color const COL_FOREGROUND { D2D1::ColorF::Black };
	m_hr = m_pRenderTarget->CreateSolidColorBrush(COL_FOREGROUND, &m_pBrushForeground);
	assert(SUCCEEDED(m_hr));

	static Color const COL_BACKGROUND { D2D1::ColorF::White };
	m_hr = m_pRenderTarget->CreateSolidColorBrush(COL_BACKGROUND, &m_pBrushBackground);
	assert(SUCCEEDED(m_hr));
}

bool D2D_driver::startFrame()
{
	assert(m_pRenderTarget != nullptr);
	m_pRenderTarget->BeginDraw();
	return true;
}

// Finish rendering; page flip.

void D2D_driver::endFrame()
{
	m_hr = m_pRenderTarget->EndDraw();
	if (m_hr == D2DERR_RECREATE_TARGET)
	{
		m_hr = S_OK;
		discardResources();
		createResources();
	}
}

void D2D_driver::Display(function<void()> func)
{
	PAINTSTRUCT ps;
	BeginPaint(m_hwnd, &ps);
	if (startFrame())
	{
		m_pRenderTarget->Clear(m_pBrushBackground->GetColor());
		func();
		endFrame();
	}
	EndPaint(m_hwnd, &ps);
}

void D2D_driver::discardResources()
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pBrushForeground);
	SafeRelease(&m_pSink);
}

unique_ptr<D2D_driver> D2D_driver::Create(HWND const hwnd)
{
	unique_ptr<D2D_driver> upGraphics { make_unique<D2D_driver>() };
	upGraphics->m_hwnd = hwnd;
	upGraphics->createResources();
	return upGraphics;
}

void D2D_driver::Resize(PIXEL const width, PIXEL const height)
{
	if (m_pRenderTarget)
	{
		HRESULT hres = m_pRenderTarget->Resize(D2D1::SizeU(width.GetValue(), height.GetValue()));
		assert(hres == S_OK);
	}
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

void D2D_driver::DisplayText
(
	fPixelRect         const& rect,
	wstring            const& wstr,
	ID2D1Brush         const& brush,
	IDWriteTextFormat* const  pTextFormat
) const
{
	IDWriteTextFormat* pTF    { pTextFormat ? pTextFormat : m_pTextFormat };
	D2D1_RECT_F const  d2Rect { convertD2D(rect) };
	m_pRenderTarget->DrawText(wstr.c_str(), static_cast<UINT32>(wstr.length()), pTF, d2Rect, const_cast<ID2D1Brush*>(&brush));
}

void D2D_driver::DisplayText
(
	fPixelRect         const& rect,
	wstring            const& wstr,
	Color              const  colF,
	IDWriteTextFormat* const  pTextFormat
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	DisplayText(rect, wstr, *pBrush, pTextFormat);
	SafeRelease(&pBrush);
}

void D2D_driver::DisplayText
(
	fPixelRect         const& rect,
	wstring            const& wstr,
	IDWriteTextFormat* const  pTextFormat
) const
{
	DisplayText(rect, wstr, *m_pBrushForeground, pTextFormat);
}

void D2D_driver::DisplayText
(
	wstring            const& wstr,
	ID2D1Brush         const& brush,
	IDWriteTextFormat* const  pTextFormat
) const
{
	IDWriteTextFormat* pTF    { pTextFormat ? pTextFormat : m_pTextFormat };
	fPixelRect  const  rect   { GetClRect() };
	D2D1_RECT_F const  d2Rect { convertD2D(rect) };
	m_pRenderTarget->DrawText(wstr.c_str(), static_cast<UINT32>(wstr.length()), pTF, d2Rect, const_cast<ID2D1Brush*>(&brush));
}

void D2D_driver::DisplayText
(
	wstring            const& wstr,
	Color              const  colF,
	IDWriteTextFormat* const  pTextFormat
) const
{
	DisplayText(GetClRect(), wstr, colF, pTextFormat);
}

void D2D_driver::DisplayText
(
	wstring            const& wstr,
	IDWriteTextFormat* const  pTextFormat
) const
{
	DisplayText(wstr, *m_pBrushForeground, pTextFormat);
}

void D2D_driver::DrawRectangle(fPixelRect const& rect, Color const colF, fPixel const fPixWidth) const
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

void D2D_driver::FillRectangle(fPixelRect const& rect, Color const colF) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	FillRectangle(rect, pBrush);
	SafeRelease(&pBrush);
}

void D2D_driver::FillRectangle(fPixelRect const& rect, ID2D1Brush * const pBrush) const
{
	m_pRenderTarget->FillRectangle(convertD2D(rect), pBrush);
}

void D2D_driver::ClearRectangle(fPixelRect const& rect) const
{
	m_pRenderTarget->FillRectangle(convertD2D(rect), m_pBrushBackground);
}

void D2D_driver::DrawRoundedRectangle
(
	fPixelRect   const& rect, 
	Color        const  colF, 
	fPixel       const  fPixRadius,
	fPixel       const  fPixStrokeWidth
) const
{
	ID2D1SolidColorBrush* pBrush    { CreateBrush(colF) };
	D2D1_ROUNDED_RECT     roundRect { convertD2D(rect), fPixRadius.GetValue(), fPixRadius.GetValue() };
	m_pRenderTarget->DrawRoundedRectangle(&roundRect, pBrush, fPixStrokeWidth.GetValue(), NULL);
	SafeRelease(&pBrush);
}

void D2D_driver::FillRoundedRectangle
(
	fPixelRect   const& rect, 
	Color const  colF, 
	fPixel       const  fPixRadius
) const
{
	ID2D1SolidColorBrush* pBrush    { CreateBrush(colF) };
	D2D1_ROUNDED_RECT     roundRect { convertD2D(rect), fPixRadius.GetValue(), fPixRadius.GetValue() };
	m_pRenderTarget->FillRoundedRectangle(&roundRect, pBrush);
	SafeRelease(&pBrush);
}

ID2D1GradientStopCollection * D2D_driver::simpleGradientStopCollection
(
	Color const colF1,
	Color const colF2 
) const
{
	HRESULT hr;
	ID2D1GradientStopCollection * pGradientStopColl = nullptr;
	array<D2D1_GRADIENT_STOP, 2>  gradientStops { D2D1_GRADIENT_STOP(0.0f, colF1), D2D1_GRADIENT_STOP(1.0f, colF2) };
	hr = m_pRenderTarget->CreateGradientStopCollection(gradientStops.data(), 2, &pGradientStopColl);
	assert(SUCCEEDED(hr));
	return pGradientStopColl;
}

void D2D_driver::FillGradientRect
(
	fPixelRect const & rect, 
	Color      const   colF1,
	Color      const   colF2 
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
	Color  const   colF1,
	Color  const   colF2 
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
	Color const   colF1,
	Color const   colF2 
) const
{
	FillGradientEllipse(fPixelEllipse { circle }, colF1, colF2);
}

void D2D_driver::DrawLine
(
	fPixelLine const& fPixLine,
	fPixel     const  fpixWidth,
	ID2D1Brush const& brush
) const
{
	m_pRenderTarget->DrawLine
	(
		convertD2D(fPixLine.GetStartPoint()),
		convertD2D(fPixLine.GetEndPoint()),
		const_cast<ID2D1Brush*>(&brush),
		fpixWidth.GetValue()
	);
}

void D2D_driver::DrawLine
(
	fPixelPoint const& fpp1,
	fPixelPoint const& fpp2,
	fPixel      const  fpixWidth,
	ID2D1Brush  const& brush
) const
{
	m_pRenderTarget->DrawLine
	(
		convertD2D(fpp1),
		convertD2D(fpp2),
		const_cast<ID2D1Brush*>(&brush),
		fpixWidth.GetValue()
	);
}

void D2D_driver::DrawLine
(
	fPixelPoint  const& fpp1,
	fPixelPoint  const& fpp2,
	fPixel       const  fpixWidth,
	Color const  colF
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	DrawLine(fpp1, fpp2, fpixWidth, *pBrush);
	SafeRelease(&pBrush);
}

void D2D_driver::DrawLine
(
	fPixelPoint const & fpp1, 
	fPixelPoint const & fpp2, 
	fPixel      const   fpixWidth
) const
{
	DrawLine(fpp1, fpp2, fpixWidth, *m_pBrushForeground);
}

void D2D_driver::FillCircle
(
	fPixelCircle const& circle,
	ID2D1Brush * const pBrush
) const
{
	FillEllipse(fPixelEllipse { circle }, pBrush);
}

void D2D_driver::FillCircle
(
	fPixelCircle const& circle,
	Color const   colF
) const
{
	FillEllipse(fPixelEllipse { circle }, colF);
}

void D2D_driver::FillCircle
(
	fPixelCircle const& circle
) const
{
	FillEllipse(fPixelEllipse { circle });
}

void D2D_driver::DrawCircle
(
	fPixelCircle const& circle,
	ID2D1Brush * const  pBrush,
	fPixel       const  fPixWidth
) const
{
	DrawEllipse(fPixelEllipse { circle }, pBrush, fPixWidth);
}

void D2D_driver::DrawCircle
(
	fPixelCircle const& circle,
	Color const  colF,
	fPixel       const  fPixWidth
) const
{
	DrawEllipse(fPixelEllipse { circle }, colF, fPixWidth);
}

void D2D_driver::DrawCircle
(
	fPixelCircle const& circle,
	fPixel       const  fPixWidth
) const
{
	DrawEllipse(fPixelEllipse { circle }, fPixWidth);
}

void D2D_driver::FillEllipse
(
	fPixelEllipse const& fPE,
	ID2D1Brush* const pBrush
) const
{
	m_pRenderTarget->FillEllipse(convertD2D(fPE), pBrush);
}

void D2D_driver::FillEllipse
(
	fPixelEllipse const& fPE,
	Color  const  colF
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	FillEllipse(fPE, pBrush);
	SafeRelease(&pBrush);
}

void D2D_driver::FillEllipse(fPixelEllipse const& fPE) const
{
	FillEllipse(fPE, m_pBrushForeground);
}

void D2D_driver::DrawEllipse
(
	fPixelEllipse const& fPE,
	ID2D1Brush*   const  pBrush,
	fPixel        const  fPixWidth
) const
{
	m_pRenderTarget->DrawEllipse(convertD2D(fPE), pBrush, fPixWidth.GetValue(), nullptr);
}

void D2D_driver::DrawEllipse
(
	fPixelEllipse const& fPE,
	Color  const   colF,
	fPixel        const   fPixWidth
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	DrawEllipse(fPE, pBrush, fPixWidth);
	SafeRelease(&pBrush);
}

void D2D_driver::DrawEllipse
(
	fPixelEllipse const& fPE,
	fPixel        const  fPixWidth
) const
{
	DrawEllipse(fPE, m_pBrushForeground, fPixWidth);
}

void D2D_driver::FillArrow
(
	fPixelPoint  const ptPos,
	fPixelPoint  const ptVector,
	fPixel       const fPixSize,  
	fPixel       const fPixWidth, 
	Color const colF
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
	Color        const   color
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
	Color        const colF
) const
{
	fPixelPoint const ptOffset(fPixSize, fPixSize);
	fPixelPoint const ptStart(ptPos - ptOffset);
	fPixelPoint const ptEnd  (ptPos + ptOffset);
	DrawLine(ptStart, ptEnd, fPixSize * sqrtf(8.0f), colF);
}

ID2D1SolidColorBrush* D2D_driver::CreateBrush(Color const d2dCol) const
{
	ID2D1SolidColorBrush* pBrush;
	HRESULT hres = m_pRenderTarget->CreateSolidColorBrush(d2dCol, &pBrush);
	assert(hres == S_OK);
	return pBrush;
}

void D2D_driver::SetForegroundColor(Color const d2dCol)
{
	m_pBrushForeground->SetColor(d2dCol);
}

void D2D_driver::SetBackgroundColor(Color const d2dCol)
{
	m_pBrushBackground->SetColor(d2dCol);
}

Color D2D_driver::GetForegroundColor() const
{ 
	D2D1_COLOR_F colF = m_pBrushForeground->GetColor();
	return *reinterpret_cast<Color *>(&colF);
}

Color D2D_driver::GetBackgroundColor() const
{
	D2D1_COLOR_F colF = m_pBrushBackground->GetColor();
	return *reinterpret_cast<Color*>(&colF);
}

void D2D_driver::SetRotation(float const fAngle, fPixelPoint const& ptCenter) const
{
	m_pRenderTarget->SetTransform
	(
		D2D1::Matrix3x2F::Rotation(fAngle, convertD2D(ptCenter))
	);
}

void D2D_driver::Reset() const
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

D2D1_POINT_2F convertD2D(fPixelPoint const & fPP)
{
	return D2D1_POINT_2F { fPP.GetXvalue(), fPP.GetYvalue() }; 
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

void D2D_driver::DrawBezier
(
	fPixelPoint     const& fPixPnt0,
	fPixelPoint     const& fPixPnt1,
	fPixelPoint     const& fPixPnt2,
	fPixelPoint     const& fPixPnt3,
	ID2D1SolidColorBrush * pBrush,
	fPixel          const  fPixWidth
) const
{
	ID2D1PathGeometry* m_pPathGeometry { nullptr };
	m_hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	assert(SUCCEEDED(m_hr));
	m_hr = m_pPathGeometry->Open(&m_pSink);
	assert(SUCCEEDED(m_hr));
	m_pSink->BeginFigure(convertD2D(fPixPnt0), D2D1_FIGURE_BEGIN_HOLLOW);
	m_pSink->AddBezier(D2D1::BezierSegment(convertD2D(fPixPnt1), convertD2D(fPixPnt2), convertD2D(fPixPnt3)));
	m_pSink->EndFigure(D2D1_FIGURE_END_OPEN);
	m_hr = m_pSink->Close();
	SafeRelease(&m_pSink);
	m_pRenderTarget->DrawGeometry(m_pPathGeometry, pBrush, fPixWidth.GetValue());
	SafeRelease(&m_pPathGeometry);
}

void D2D_driver::DrawBezier
(
	fPixelPoint const& fPixPnt0,
	fPixelPoint const& fPixPnt1,
	fPixelPoint const& fPixPnt2,
	fPixelPoint const& fPixPnt3,
	fPixel      const  fPixWidth
) const
{
	DrawBezier(fPixPnt0, fPixPnt1, fPixPnt2, fPixPnt3, m_pBrushForeground, fPixWidth);
}

void D2D_driver::DrawBezier
(
	fPixelPoint  const& fPixPnt0,
	fPixelPoint  const& fPixPnt1,
	fPixelPoint  const& fPixPnt2,
	fPixelPoint  const& fPixPnt3,
	Color const  col,
	fPixel       const  fPixWidth
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(col) };
	DrawBezier(fPixPnt0, fPixPnt1, fPixPnt2, fPixPnt3, pBrush, fPixWidth);
	SafeRelease(&pBrush);
}

