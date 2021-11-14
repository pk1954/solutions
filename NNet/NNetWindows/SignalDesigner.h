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
class Observable;

class SignalDesigner : public BaseWindow
{
public:
	SignalDesigner
	(
		HWND const, 
		ComputeThread const &, 
		SignalGenerator &,
		Observable &
	);

	virtual ~SignalDesigner() {};

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Stop();

private:

	inline static PIXEL const LEFT_OFFSET   { 30_PIXEL };
	inline static PIXEL const BOTTOM_OFFSET { 30_PIXEL };
	inline static PIXEL const RIGHT_OFFSET  {  0_PIXEL };
	inline static PIXEL const TOP_OFFSET    {  0_PIXEL };

	//void doPaint() const;

	virtual void OnPaint() {};
	virtual bool OnSize(WPARAM const, LPARAM const);

	enum class tZoomMode  { NONE, HORZ, VERT };

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	inline static Param * m_pParameters { nullptr };

	unique_ptr<SignalControl>     m_upSignalControl;
	unique_ptr<Scale<fMicroSecs>> m_upHorzScale;
	unique_ptr<Scale<fHertz>>     m_upVertScale;

	D2D_driver             m_graphics      { };
	tZoomMode              m_zoomMode      { tZoomMode::NONE };
	fPixel                 m_fPixLineWidth { 1.0_fPixel };
	PixCoordFp<fMicroSecs> m_horzCoord;
	PixCoordFp<fHertz>     m_vertCoord;

	inline static fPixel const DIAMOND_SIZE { 4.0_fPixel };
};