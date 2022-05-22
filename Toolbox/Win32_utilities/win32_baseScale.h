// win32_baseScale.h
//
// Toolbox\win32_utilities

#pragma once

#include <string> 
#include "util.h"
#include "MoreTypes.h"
#include "Direct2D.h"
#include "dwrite.h"
#include "PixFpDimension.h"
#include "win32_util_resource.h"
#include "win32_graphicsWindow.h"

using std::wstring;
using std::bit_cast;

class D2D_driver;

struct IDWriteTextFormat;

class BaseScale : public GraphicsWindow
{
public:
	BaseScale(HWND const, bool const);

	virtual bool Zoom(bool  const) { return false; }
	virtual void Zoom(float const) {}

	bool         IsVertScale   () const { return m_bVertScale; }
	bool         GetOrientation() const { return m_bOrientation; }
	fPixel       GetLeftBorder () const { return m_fPixLeftBorder; }
	fPixel       GetRightBorder() const { return m_fPixRightBorder; }
	fPixel       GetOrthoOffset() const { return m_fPixOrthoOffset; }
	D2D1::ColorF GetColor()       const { return m_scaleColor; }

	void SetRightBorder(fPixel       const b  ) { m_fPixRightBorder = b;   }
	void SetScaleColor (D2D1::ColorF const col) { m_scaleColor      = col; }
	void SetOrthoOffset(fPixel       const off) { m_fPixOrthoOffset = off; }

	void SetOrientation(bool const); 

	void OnMouseMove      (WPARAM const, LPARAM const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	void OnMouseWheel     (WPARAM const, LPARAM const) final;
	bool OnSize           (PIXEL  const, PIXEL  const) final;
	void OnMouseLeave     () final;

	inline static D2D1::ColorF const COL_NORMAL      { D2D1::ColorF::Azure };
	inline static D2D1::ColorF const COL_HIGHLIGHTED { D2D1::ColorF::Aquamarine };

protected:

	inline static fPixel const LONG_TICK   { 10._fPixel };
	inline static fPixel const MIDDLE_TICK {  7._fPixel };
	inline static fPixel const SMALL_TICK  {  5._fPixel };

	void   display   (fPixelRect const &, wstring const &) const;
	void   setTextBox(fPixelRect & textBox) const;
	fPixel getClHeight() const { return m_upGraphics->GetClRectHeight(); }

private:

	D2D1::ColorF        m_scaleColor      { D2D1::ColorF::Black };
	IDWriteTextFormat * m_pTextFormat     { nullptr };
	bool                m_bOrientation    { true };  // true: ticks on negative side of scale
	bool                m_bVertScale      { false }; // true: vertical, false: horizontal
	fPixel              m_fPixLeftBorder  { 0._fPixel };
	fPixel              m_fPixRightBorder { 0._fPixel };
	fPixel              m_fPixOrthoOffset { 0._fPixel };
};

