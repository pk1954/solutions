// Color.ixx
//
// Toolbox\Utilities

module;

#include "d2d1helper.h"

export module Color;

import std;

using std::wstring;
using std::wostream;

export using D2D1::ColorF;

export using BrushHandle      = ID2D1Brush *;
export using TextFormatHandle = IDWriteTextFormat *;

export inline COLORREF Convert2COLORREF(D2D1_COLOR_F const col)
{
	COLORREF colorRef = RGB
	(
		static_cast<BYTE>(col.r * 255.0f),
		static_cast<BYTE>(col.g * 255.0f),
		static_cast<BYTE>(col.b * 255.0f)
	);
	return colorRef;
}

export inline D2D1_COLOR_F Convert2ColorF(COLORREF const color)
{
	D2D1_COLOR_F colF
	(
		static_cast<float>(GetRValue(color)) / 255.0f,
		static_cast<float>(GetGValue(color)) / 255.0f,
		static_cast<float>(GetBValue(color)) / 255.0f,
		1.0f
	);
	return colF;
}

export class Color : public D2D1_COLOR_F
{
public:
	Color()
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		a = 1.0f;
	}

	Color
	(
		float const rPar,
		float const gPar,
		float const bPar,
		float const aPar = 1.0f
	)
	{
		r = rPar;
		g = gPar;
		b = bPar;
		a = aPar;
	}

	Color(D2D1::ColorF col)
	{
		r = col.r;
		g = col.g;
		b = col.b;
		a = 1.0f;
	}

	Color(D2D1::ColorF::Enum rgb)
	{
		r = static_cast<float>(GetBValue(rgb)) / 255.0f;
		g = static_cast<float>(GetGValue(rgb)) / 255.0f;
		b = static_cast<float>(GetRValue(rgb)) / 255.0f;
		a = 1.0f;
	}

	Color
	(
		unsigned short const red,
		unsigned short const green,
		unsigned short const blue
	)
	{
		r = static_cast<float>(red  ) / 255.0f;
		g = static_cast<float>(green) / 255.0f;
		b = static_cast<float>(blue ) / 255.0f;
		a = 1.0f;
	}

	Color(COLORREF rgb)
	{
		r = static_cast<float>(GetRValue(rgb)) / 255.0f;
		g = static_cast<float>(GetGValue(rgb)) / 255.0f;
		b = static_cast<float>(GetBValue(rgb)) / 255.0f;
		a = 1.0f;
	}

	Color& operator+= (Color const other)
	{ 
		r += other.r;
		g += other.g;
		b += other.b;
		return *this;
	}

	Color& operator-= (Color const other) 
	{
		r -= other.r;
		g -= other.g;
		b -= other.b;
		return *this;
	}

	Color& operator*= (float const f)
	{
		r = min(1.0f, r*f);
		g = min(1.0f, g*f);
		b = min(1.0f, b*f);
		return *this;
	}

	Color& operator/= (float const f)
	{
		r = min(1.0f, r/f);
		g = min(1.0f, g/f);
		b = min(1.0f, b/f);
		return *this;
	}

	friend Color operator+ (Color const a, Color const b)
	{
		Color res { a };
		res += b;
		return res;
	}

	friend Color operator- (Color const a, Color const b)
	{
		Color res { a };
		res -= b;
		return res;
	}

	friend Color operator* (Color const a, float const f)
	{
		Color res { a };
		res *= f;
		return res;
	}

	friend Color operator/ (Color const a, float const f)
	{
		Color res { a };
		res /= f;
		return res;
	}
};

export inline Color const COL_BLACK { 1.0f, 1.0f, 1.0f, 1.0f };
