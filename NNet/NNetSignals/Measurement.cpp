// Measurement.cpp
//
// NNetSignals

module;

#include <sstream> 
#include <iomanip>
#include <dwrite.h>

module NNetSignals:Measurement;

import Types;
import Util;
import Direct2D;

using std::wostringstream;
using std::setprecision;
using std::fixed;
using std::endl;

static D2D1::ColorF const COL_STRONG { 0.0f, 0.4f, 0.0f, 1.0f };
static D2D1::ColorF const COL_WEAK   { 0.0f, 1.0f, 0.0f, 0.2f };

void Measurement::Initialize(D2D_driver * const pGraphics)
{
	m_pGraphics   = pGraphics;
	m_pTextFormat = pGraphics->NewTextFormat(12.f);
}

void Measurement::ResetLimits()
{
	if (m_fPixClientWidth.IsNotNull())
	{
		m_fPixSelLeft  = m_fPixClientWidth * 0.1f;
		m_fPixSelRight = m_fPixClientWidth * 0.9f;
	}
}

void Measurement::SetClientRectSize(PIXEL const width, PIXEL const height)
{
	m_fPixClientWidth  = Convert2fPixel(width);
	m_fPixClientHeight = Convert2fPixel(height);
	ResetLimits();
}

bool Measurement::Select(fPixel const fPix)
{
	if (m_bActive)
	{
		m_bLeftActive  = IsClose2LeftLimit (fPix);
		m_bRightActive = IsClose2RightLimit(fPix);
		if (m_bLeftActive || m_bRightActive)
			return true;
	}
	return false;
}

void Measurement::MoveSelection(fPixel const fPix)
{
	if (m_bActive)
	{
		if (m_bLeftActive && (fPix < m_fPixSelRight))
			m_fPixSelLeft = fPix;
		else if (m_bRightActive && (fPix > m_fPixSelLeft))
			m_fPixSelRight = fPix;
	}
}

void Measurement::verticalLine(fPixel const fPixPosX) const
{
	m_pGraphics->DrawLine
	(
		fPixelPoint(fPixPosX, 0._fPixel), 
		fPixelPoint(fPixPosX, m_fPixClientHeight), 
		1._fPixel, 
		D2D1::ColorF::Black
	);
}

void Measurement::emphasizedLineLeft(fPixel const fPixPosX) const
{
	fPixelRect const rect
	{ 
		fPixPosX, 
		0._fPixel, 
		fPixPosX + GRADIENT_WIDTH, 
		m_fPixClientHeight 
	};

	m_pGraphics->FillGradientRect(rect, COL_STRONG, COL_WEAK  );
}

void Measurement::emphasizedLineRight(fPixel const fPixPosX) const
{
	fPixelRect const rect
	{ 
		fPixPosX - GRADIENT_WIDTH, 
		0._fPixel, 
		fPixPosX, 
		m_fPixClientHeight 
	};

	m_pGraphics->FillGradientRect(rect, COL_WEAK, COL_STRONG);
}

void Measurement::measuringArea() const
{
	fPixelRect const rect
	{ 
		m_fPixSelLeft + (m_bLeftActive ? GRADIENT_WIDTH : 0.0_fPixel), 
		0._fPixel, 
		m_fPixSelRight - (m_bRightActive ? GRADIENT_WIDTH : 0.0_fPixel),
		m_fPixClientHeight 
	};
	m_pGraphics->FillRectangle(rect, COL_WEAK);
}

void Measurement::textArea(fMicroSecs const fMicroSecsPerPixel) const
{
	fPixel     const fPixDistance { m_fPixSelRight - m_fPixSelLeft };
	fMicroSecs const usMeasured   { fMicroSecsPerPixel * fPixDistance.GetValue() };
	fHertz     const frequency    { Frequency(usMeasured) };

	fPixel fPosRight { m_fPixSelRight - GRADIENT_WIDTH };
	fPixel fPosTop   { GRADIENT_WIDTH };

	fPixelRect fPixRect
	(
		fPosRight - 70._fPixel, // left
		fPosTop,                // top
		fPosRight,              // right
		fPosTop + 35._fPixel    // bottom
	);

	wostringstream wBuffer;
	wBuffer << Format2wstring(usMeasured) << endl;
	wBuffer << Format2wstring(frequency);

	static D2D1::ColorF const COLOR_TEXT     { D2D1::ColorF::Black }; 
	static D2D1::ColorF const COL_BACKGROUND { D2D1::ColorF::AntiqueWhite };

	m_pGraphics->FillRectangle(fPixRect, COL_BACKGROUND);
	m_pGraphics->DisplayText  (fPixRect, wBuffer.str(), COLOR_TEXT, m_pTextFormat);
}

bool Measurement::IsClose2LeftLimit (fPixel const fPix) const 
{ 
	return (m_fPixSelLeft <= fPix) && (fPix <= m_fPixSelLeft + GRADIENT_WIDTH);
}

bool Measurement::IsClose2RightLimit(fPixel const fPix) const 
{ 
	return (m_fPixSelRight - GRADIENT_WIDTH <= fPix) && (fPix <= m_fPixSelRight);
}

void Measurement::DisplayDynamicScale(fMicroSecs const fMicroSecsPerPixel) const
{
	if (m_bActive)
	{
		emphasizedLineLeft(m_fPixSelLeft);
		verticalLine(m_fPixSelLeft);
		measuringArea();
		emphasizedLineRight(m_fPixSelRight);
		verticalLine(m_fPixSelRight);
		textArea(fMicroSecsPerPixel);
	}
}
