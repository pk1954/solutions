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

	void SetVoltColor(D2D1::ColorF const col)
	{
		m_colorVolt = col;
	}

private:

	void paintCurve(auto getPoint, D2D1::ColorF const col) const
	{
		fMicroSecs const usResolution { m_signalGenerator.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
		fMicroSecs const usPixelSize  { m_horzCoord.GetPixelSize() };
		fMicroSecs const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
		fMicroSecs const timeStart    { 0.0_MicroSecs };
		fMicroSecs const timeCutoff   { m_signalGenerator.CutoffTime() };
		fMicroSecs       timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
		fPixelPoint      prevPoint    { getPoint(timeStart) };

		if (timeCutoff < timeEnd)
			timeEnd = timeCutoff;

		for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
		{
			fPixelPoint const actPoint  { getPoint(time) };
			fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
			m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, col);
			m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, col);
			prevPoint = actPoint;
		}
	}

	void DoPaint() final;

	bool OnSize      (PIXEL  const, PIXEL  const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;

	fMicroSecs getTime(fPixel const) const;
	fHertz     getFreq(fPixel const) const;
	mV         getVolt(fPixel const) const;

	fPixel getPixX(fMicroSecs const) const;
	fPixel getPixYfreq(fHertz const) const;
	fPixel getPixYvolt(mV     const) const;
	fPixel getPixXmax() const;
	fPixel getPixYmaxFreq() const;
	fPixel getPixYmaxVolt() const;
	fPixel getPixYbase() const;

	fPixelPoint getPixPntFreq  (fMicroSecs const, fHertz const) const;
	fPixelPoint getPixPntVolt  (fMicroSecs const, mV     const) const;
	fPixelPoint getGraphPntFreq(fMicroSecs const) const;
	fPixelPoint getGraphPntVolt(fMicroSecs const) const;
	fPixelPoint getPixPntMaxFreq() const;
	fPixelPoint getPixPntMaxVolt() const;

	bool baseLineSelected   (fPixelPoint const &) const;
	bool freqMaxLineSelected(fPixelPoint const &) const;
	bool currMaxLineSelected(fPixelPoint const &) const;
	bool timeMaxLineSelected(fPixelPoint const &) const;

	void displayBaseFrequency(D2D1::ColorF const, fPixel const) const;
	void displayFreqLine     (fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayVoltLine     (fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayTimeLine     (fMicroSecs const, fPixel const, D2D1::ColorF const) const;
	void displayFreqMaxPoint (fMicroSecs const, fPixel const, D2D1::ColorF const, fPixel const) const;
	void displayVoltMaxPoint (fMicroSecs const, fPixel const, D2D1::ColorF const, fPixel const) const;

	void highlightMovedObject() const;

	enum class tMoveMode { NONE, MAX_FREQ_PNT, MAX_FREQ, MAX_TIME, BASE_FREQ, MAX_CURR_PNT, MAX_CURR };
	enum class tZoomMode { NONE, HORZ, VERT };

	inline static fPixel const DIAMOND_SIZE   {  4.0_fPixel };
	inline static fPixel const LINE_WIDTH_STD {  1.0_fPixel };
	inline static fPixel const RECOG_DIST     { 10.0_fPixel };

	inline static Param * m_pParameters { nullptr };

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<fHertz>     & m_vertCoordFreq;
	PixFpDimension<mV>         & m_vertCoordVolt;
	ComputeThread        const & m_computeThread;
	SignalGenerator            & m_signalGenerator;
	NNetModelCommands          & m_commands;
	SignalGenerator              m_sigGenNew;
	tMoveMode                    m_moveMode       { tMoveMode::NONE };
	fPixel                       m_fPixGraphWidth { 0.0_fPixel };
	fPixel                       m_fPixLineWidth  { 1.0_fPixel };
	D2D1::ColorF                 m_colorFreq      { D2D1::ColorF::Black };
	D2D1::ColorF                 m_colorVolt      { D2D1::ColorF::Black };
};