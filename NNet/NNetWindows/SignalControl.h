// SignalControl.h
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixCoordFp.h"
#include "NNetParameters.h"
#include "win32_baseWindow.h"

class SignalGenerator;
class ComputeThread;
class NNetModelCommands;

class SignalControl : public BaseWindow
{
public:
	SignalControl
	(
		HWND             const, 
		ComputeThread    const &, 
		SignalGenerator        &,
		PixCoordFp<fMicroSecs> &,
		PixCoordFp<fHertz>     &,
		NNetModelCommands      &
	);

	~SignalControl() final;

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Reset();
	void Stop();

private:

	inline static D2D1::ColorF const COL_STD { D2D1::ColorF::Green };
	inline static D2D1::ColorF const COL_HI  { D2D1::ColorF::Red   };
	inline static D2D1::ColorF const COL_LOW { D2D1::ColorF::LightGray };

	inline static fPixel const LINE_WIDTH_STD { 1.0_fPixel };
	inline static fPixel const LINE_WIDTH_HI  { 3.0_fPixel };

	void doPaint() const;

	void OnPaint     () final;
	bool OnSize      (WPARAM const, LPARAM const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;
	bool OnCommand   (WPARAM const, LPARAM const, PixelPoint const) final;

	fMicroSecs getTime(fPixel const) const;
	fHertz     getFreq(fPixel const) const;

	fPixel getPixX(fMicroSecs const) const;
	fPixel getPixY(fHertz     const) const;
	fPixel getPixXmax() const;
	fPixel getPixYmax() const;
	fPixel getPixYbase() const;

	fPixelPoint getPixPnt  (fMicroSecs const, fHertz const) const;
	fPixelPoint getGraphPnt(fMicroSecs const)               const;
	fPixelPoint getPixPntMax() const;

	bool baseLineSelected   (fPixelPoint const &) const;
	bool freqMaxLineSelected(fPixelPoint const &) const;
	bool timeMaxLineSelected(fPixelPoint const &) const;

	void displayBaseFrequency(fPixel const, D2D1::ColorF const) const;

	void displayFreqLine(fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayTimeLine(fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayPoint   (fMicroSecs const, fPixel const, fPixel const, D2D1::ColorF const) const;

	enum class tTrackMode { NONE, MAX_PNT, MAX_FREQ, MAX_TIME, BASE_FREQ };
	enum class tZoomMode  { NONE, HORZ, VERT };

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	inline static Param * m_pParameters { nullptr };

	NNetModelCommands      & m_commands;
	ComputeThread    const & m_computeThread;
	SignalGenerator        & m_signalGenerator;
	PixCoordFp<fMicroSecs> & m_horzCoord;
	PixCoordFp<fHertz>     & m_vertCoord;
	D2D_driver               m_graphics       { };
	tTrackMode               m_trackMode      { tTrackMode::NONE };
	fPixel                   m_fPixGraphWidth { 0.0_fPixel };
	fPixel                   m_fPixLineWidth  { 1.0_fPixel };

	inline static fPixel const DIAMOND_SIZE { 4.0_fPixel };
};