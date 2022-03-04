// SignalControl.h
//
// NNetWindows

#pragma once

#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "SignalGenerator.h"
#include "NNetParameters.h"
#include "win32_graphicsWindow.h"

class ComputeThread;
class NNetModelCommands;
class NNetModelWriterInterface;

class SignalControl : public GraphicsWindow
{
public:
	SignalControl
	(
		HWND                 const, 
		ComputeThread        const &, 
		NNetModelCommands          &,
		SignalGenerator            &,
		PixFpDimension<fMicroSecs> &,
		PixFpDimension<fHertz>     &,
		PixFpDimension<mV>         &
	);

	~SignalControl() final;

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void SetFreqColor(D2D1::ColorF const col)
	{
		m_colorFreq = col;
	}

	void SetCurrColor(D2D1::ColorF const col)
	{
		m_colorCurr = col;
	}

private:

	void DoPaint() final;

	bool OnSize      (PIXEL  const, PIXEL  const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;

	fMicroSecs getTime(fPixel const) const;
	fHertz     getFreq(fPixel const) const;

	fPixel getPixX(fMicroSecs const) const;
	fPixel getPixY(fHertz     const) const;
	fPixel getPixXmax() const;
	fPixel getPixYmax() const;
	fPixel getPixYbase() const;

	fPixelPoint getPixPnt  (fMicroSecs const, fHertz const) const;
	fPixelPoint getGraphPnt(fMicroSecs const) const;
	fPixelPoint getPixPntMax() const;

	bool baseLineSelected   (fPixelPoint const &) const;
	bool freqMaxLineSelected(fPixelPoint const &) const;
	bool timeMaxLineSelected(fPixelPoint const &) const;

	void displayBaseFrequency(D2D1::ColorF const, fPixel const) const;
	void displayFreqLine     (fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayTimeLine     (fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayPoint        (fMicroSecs const, fPixel const, fPixel const, D2D1::ColorF const) const;

	void paintSigGenData(D2D1::ColorF const) const;
	void highlightMovedObject() const;

	enum class tMoveMode { NONE, MAX_PNT, MAX_FREQ, MAX_TIME, BASE_FREQ };
	enum class tZoomMode { NONE, HORZ, VERT };

	inline static fPixel const DIAMOND_SIZE   {  4.0_fPixel };
	inline static fPixel const LINE_WIDTH_STD {  1.0_fPixel };
	inline static fPixel const RECOG_DIST     { 10.0_fPixel };

	inline static Param * m_pParameters { nullptr };

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<fHertz>     & m_vertCoordFreq;
	PixFpDimension<mV>         & m_vertCoordCurr;
	ComputeThread        const & m_computeThread;
	SignalGenerator            & m_signalGenerator;
	NNetModelCommands          & m_commands;
	SignalGenerator              m_sigGenNew;
	tMoveMode                    m_moveMode       { tMoveMode::NONE };
	fPixel                       m_fPixGraphWidth { 0.0_fPixel };
	fPixel                       m_fPixLineWidth  { 1.0_fPixel };
	D2D1::ColorF                 m_colorFreq      { D2D1::ColorF::Black };
	D2D1::ColorF                 m_colorCurr      { D2D1::ColorF::Black };
};