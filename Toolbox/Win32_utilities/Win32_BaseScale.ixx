// Win32_BaseScale.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h> 

export module BaseScale;

import std;
import Win32_Util_Resource;
import BoolOp;
import Types;
import Direct2D;
import PixFpDimension;
import GraphicsWindow;

using std::wstring;
using std::bit_cast;

export class BaseScale : public GraphicsWindow
{
public:
	BaseScale(HWND const, bool const);

	virtual bool ZoomCoordFactor(float const, fPixel const) = 0;
	virtual void MoveCoord(PIXEL const) = 0;

	void SetDefaultBackgroundColor() override;

	bool   IsInverted     () const { return m_bInverted; }
	bool   IsVertScale    () const { return m_bVertScale; }
	bool   GetTicksDir    () const { return m_bTicksDir; }
	fPixel GetLeftBorder  () const { return m_fPixLeftBorder; }
	fPixel GetRightBorder () const { return m_fPixRightBorder; }
	fPixel GetTopBorder   () const { return m_fPixTopBorder; }
	fPixel GetBottomBorder() const { return m_fPixBottomBorder; }
	fPixel GetOrthoOffset () const { return m_fPixOrthoOffset; }

	void SetTopBorder   (fPixel const b)   { m_fPixTopBorder    = b; }
	void SetBottomBorder(fPixel const b)   { m_fPixBottomBorder = b; }
	void SetRightBorder (fPixel const b)   { m_fPixRightBorder  = b; }
	void SetLeftBorder  (fPixel const b)   { m_fPixLeftBorder   = b; }
	void SetOrthoOffset (fPixel const off) { m_fPixOrthoOffset  = off; }

	void SetScaleColor(Color const c) { m_upGraphics->SetForegroundColor(c); }

	inline static bool const TICKS_UP    { true };
	inline static bool const TICKS_DOWN  { false };
	inline static bool const TICKS_RIGHT { false };
	inline static bool const TICKS_LEFT  { true };

	void SetTicksDir   (bool const);
	void SetInverted   (bool const b)         { m_bInverted      = b; }
	void SetZoomAllowed(bool const b)         { m_bZoomAllowed   = b; }
	void SetAllowUnlock(bool const b)         { m_bUnlockAllowed = b; }
	void SetBlock2Zero (bool const b)         { m_bLock2Zero     = b; }
	void SetBlock2Zero (tBoolOp const op)     { ApplyOp(m_bLock2Zero, op); }
	void SetUnitOffset (fPixelPoint const &p) { m_fPixUnitOffset = p; }

	bool IsScaleLocked() const { return m_bLock2Zero; }
	bool IsZoomAllowed() const { return m_bZoomAllowed; }

	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) override;
	void OnMouseMove      (WPARAM const, LPARAM const)                   override;
	bool OnSize           (PIXEL  const, PIXEL  const)                   override;
	void OnLButtonDblClick(WPARAM const, LPARAM const)                   override;
	bool OnLButtonDown    (WPARAM const, LPARAM const)                   override;
	bool OnLButtonUp      (WPARAM const, LPARAM const)                   override;
	void OnMouseLeave     () override;

	void Notify(bool const) override;

protected:

	inline static fPixel const LONG_TICK   { 10._fPixel };
	inline static fPixel const MIDDLE_TICK { 7._fPixel };
	inline static fPixel const SMALL_TICK  { 5._fPixel };

	void   display(fPixelRect const&, wstring const&) const;
	void   setTextBox(fPixelRect& textBox) const;
	fPixel getClHeight() const { return m_upGraphics->GetClRectHeight(); }
	fPixel getClWidth () const { return m_upGraphics->GetClRectWidth (); }

	fPixel xPos(fPixel const x) const { return m_bInverted ? getClWidth() - x : x; }
	fPixel yPos(fPixel const y) const { return m_bInverted ? getClHeight() - y : y; }

	fPixelPoint m_fPixUnitOffset { fPP_NULL };

private:

	TextFormatHandle m_hTextFormat      { nullptr };
	bool             m_bTicksDir        { true };     // true: ticks on negative side of scale
	bool             m_bVertScale       { false };    // true: vertical, false: horizontal
	bool             m_bInverted        { false };    // false: scale values grow from left to right / top to bottom
	fPixel           m_fPixLeftBorder   { 0._fPixel };
	fPixel           m_fPixRightBorder  { 0._fPixel };
	fPixel           m_fPixTopBorder    { 0._fPixel };
	fPixel           m_fPixBottomBorder { 0._fPixel };
	fPixel           m_fPixOrthoOffset  { 0._fPixel };
	PIXEL            m_pixLast          { PIXEL::NULL_VAL() }; // Last cursor position during selection 
	bool             m_bLock2Zero       { true };
	bool             m_bUnlockAllowed   { false };
	bool             m_bZoomAllowed     { true };

	LPARAM AddContextMenuEntries(HMENU const) override;
};
