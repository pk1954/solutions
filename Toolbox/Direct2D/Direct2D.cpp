// Direct2D.cpp
//
// Toolbox\Direct2D

module;

#include <array>
#include <string>
#include <cassert>
#include "d2d1.h"
#include "dwrite.h"

module Direct2D;

import Util;
import Win32_PIXEL;

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

	SetColor(m_color);
}

void D2D_driver::discardResources() 
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pBrush);
	SafeRelease(&m_pSink);
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
	D2D1::ColorF       const  colF,
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
	DisplayText(rect, wstr, *m_pBrush, pTextFormat);
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

void D2D_driver::FillRectangle(fPixelRect const& rect, D2D1::ColorF const colF) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	m_pRenderTarget->FillRectangle(convertD2D(rect), pBrush);
	SafeRelease(&pBrush);
}

void D2D_driver::FillRoundedRectangle(fPixelRect const& rect, D2D1::ColorF const colF, fPixel fRadius) const
{
	ID2D1SolidColorBrush* pBrush    { CreateBrush(colF) };
	D2D1_ROUNDED_RECT     roundRect { convertD2D(rect), fRadius.GetValue(), fRadius.GetValue() };
	m_pRenderTarget->FillRoundedRectangle(&roundRect, pBrush);
	SafeRelease(&pBrush);
}

ID2D1GradientStopCollection * D2D_driver::simpleGradientStopCollection
(
	D2D1::ColorF const colF1,
	D2D1::ColorF const colF2 
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
	fPixelPoint const & fpp1,
	fPixelPoint const & fpp2,
	fPixel      const   fpixWidth,
	ID2D1Brush  const & brush
) const
{
	m_pRenderTarget->DrawLine
	(
		convertD2D(fpp1),
		convertD2D(fpp2),
		const_cast<ID2D1Brush *>(&brush),
		fpixWidth.GetValue()
	);
}

void D2D_driver::DrawLine
(
	fPixelPoint  const& fpp1,
	fPixelPoint  const& fpp2,
	fPixel       const  fpixWidth,
	D2D1::ColorF const  colF
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
	DrawLine(fpp1, fpp2, fpixWidth, *m_pBrush);
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
	D2D1::ColorF const   colF
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
	D2D1::ColorF const  colF,
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
	D2D1::ColorF  const  colF
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(colF) };
	FillEllipse(fPE, pBrush);
	SafeRelease(&pBrush);
}

void D2D_driver::FillEllipse(fPixelEllipse const& fPE) const
{
	FillEllipse(fPE, m_pBrush);
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
	D2D1::ColorF  const   colF,
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
	DrawEllipse(fPE, m_pBrush, fPixWidth);
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

ID2D1SolidColorBrush* D2D_driver::CreateBrush(D2D1::ColorF const d2dCol) const
{
	ID2D1SolidColorBrush* pBrush;
	HRESULT hres = m_pRenderTarget->CreateSolidColorBrush(d2dCol, &pBrush);
	assert(hres == S_OK);
	return pBrush;
}

D2D1::ColorF D2D_driver::SetColor(D2D1::ColorF const d2dCol)
{
	D2D1::ColorF colorOld = m_color;
	m_color = d2dCol;
	SafeRelease(&m_pBrush);
	HRESULT hres = m_pRenderTarget->CreateSolidColorBrush(m_color, &m_pBrush);
	assert(hres == S_OK);
	return colorOld;
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
	DrawBezier(fPixPnt0, fPixPnt1, fPixPnt2, fPixPnt3, m_pBrush, fPixWidth);
}

void D2D_driver::DrawBezier
(
	fPixelPoint  const& fPixPnt0,
	fPixelPoint  const& fPixPnt1,
	fPixelPoint  const& fPixPnt2,
	fPixelPoint  const& fPixPnt3,
	D2D1::ColorF const  col,
	fPixel       const  fPixWidth
) const
{
	ID2D1SolidColorBrush* pBrush { CreateBrush(col) };
	DrawBezier(fPixPnt0, fPixPnt1, fPixPnt2, fPixPnt3, pBrush, fPixWidth);
	SafeRelease(&pBrush);
}

void D2D_driver::test() const
{
	ID2D1SolidColorBrush * pBrush          { CreateBrush(D2D1::ColorF::DarkBlue) };
	ID2D1PathGeometry    * m_pPathGeometry { nullptr };
	
	m_hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	assert(SUCCEEDED(m_hr));

	m_hr = m_pPathGeometry->Open(&m_pSink);
	assert(SUCCEEDED(m_hr));

	m_pSink->BeginFigure
	(
		D2D1::Point2F(0, 0),
		D2D1_FIGURE_BEGIN_FILLED
	);

	m_pSink->AddLine(D2D1::Point2F(200, 0));

	m_pSink->AddBezier
	(
		D2D1::BezierSegment
		(
			D2D1::Point2F(150, 50),
			D2D1::Point2F(150, 150),
			D2D1::Point2F(200, 200)
		)
	);

	m_pSink->AddLine(D2D1::Point2F(0, 200));

	m_pSink->AddBezier
	(
		D2D1::BezierSegment
		(
			D2D1::Point2F(50, 150),
			D2D1::Point2F(50, 50),
			D2D1::Point2F(0, 0)
		)
	);

	m_pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

	m_hr = m_pSink->Close();

	SafeRelease(&m_pSink);

	m_pRenderTarget->DrawGeometry(m_pPathGeometry, pBrush, 5);

	SafeRelease(&m_pPathGeometry);
	SafeRelease(&pBrush);
}

