// SignalDesigner.h
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "win32_scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixCoordFp.h"
#include "SignalControl.h"
#include "win32_baseWindow.h"

class SignalGenerator;
class ComputeThread;

class SignalDesigner : public BaseWindow
{
public:
	SignalDesigner(HWND const, ComputeThread const &, SignalGenerator &);
	virtual ~SignalDesigner() {};

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Reset();
	void Stop();

private:

	inline static PIXEL const LEFT_OFFSET   { 30_PIXEL };
	inline static PIXEL const BOTTOM_OFFSET { 30_PIXEL };
	inline static PIXEL const RIGHT_OFFSET  { 10_PIXEL };
	inline static PIXEL const TOP_OFFSET    { 10_PIXEL };

	inline static D2D1::ColorF const COL_STD { D2D1::ColorF::Green };
	inline static D2D1::ColorF const COL_HI  { D2D1::ColorF::Red   };
	inline static D2D1::ColorF const COL_LOW { D2D1::ColorF::LightGray };

	void doPaint() const;

	virtual void OnPaint     ();
	virtual bool OnSize      (WPARAM const, LPARAM const);
	virtual void OnMouseMove (WPARAM const, LPARAM const);
	virtual void OnMouseWheel(WPARAM const, LPARAM const);
	virtual bool OnCommand   (WPARAM const, LPARAM const, PixelPoint const);

	bool horzScaleSelected  (fPixelPoint const &) const;
	bool vertScaleSelected  (fPixelPoint const &) const;

	enum class tZoomMode  { NONE, HORZ, VERT };

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	inline static Param * m_pParameters { nullptr };

	unique_ptr<SignalControl> m_upSignalControl;

	D2D_driver             m_graphics      { };
	tZoomMode              m_zoomMode      { tZoomMode::NONE };
	fPixel                 m_fPixLineWidth { 1.0_fPixel };
	Scale<fMicroSecs>      m_horzScale;
	Scale<fHertz>          m_vertScale;
	PixCoordFp<fMicroSecs> m_horzCoord;
	PixCoordFp<fHertz>     m_vertCoord;

	inline static fPixel const DIAMOND_SIZE { 4.0_fPixel };
};