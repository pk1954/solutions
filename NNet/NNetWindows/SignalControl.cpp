// SignalControl.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "PointType.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "ComputeThread.h"
#include "SignalControl.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"

SignalControl::~SignalControl()
{
	m_signalGenerator.UnregisterObserver(*this);
}

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	NNetModelCommands          & commands,
	SignalGenerator            & signalGenerator,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<fHertz>     & vertCoordFreq,
	PixFpDimension<mV>         & vertCoordCurr
)
  :	m_horzCoord(horzCoord),
	m_vertCoordFreq(vertCoordFreq),
	m_vertCoordCurr(vertCoordCurr),
	m_computeThread(computeThread),
	m_signalGenerator(signalGenerator),
	m_commands(commands)
{
	m_signalGenerator.RegisterObserver(*this); // signal generator data can be changed fron outside
	m_horzCoord      .RegisterObserver(*this); 
	m_vertCoordFreq  .RegisterObserver(*this);
	m_vertCoordCurr  .RegisterObserver(*this);
	GraphicsWindow::Initialize(hwndParent, L"ClassSignalControl", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
}

fMicroSecs SignalControl::getTime(fPixel const fPix) const
{
	return m_horzCoord.Transform2logUnitPos(fPix);
}

fHertz SignalControl::getFreq(fPixel const fPix) const 
{
	return m_vertCoordFreq.Transform2logUnitPos(Convert2fPixel(GetClientWindowHeight()) - fPix);
}

mV SignalControl::getCurr(fPixel const fPix) const 
{
	return m_vertCoordCurr.Transform2logUnitPos(Convert2fPixel(GetClientWindowHeight()) - fPix);
}

fPixel SignalControl::getPixX(fMicroSecs const time) const
{
	return fPixel(m_horzCoord.Transform2fPixelPos(time));
}

fPixel SignalControl::getPixYfreq(fHertz const freq) const
{
	return Convert2fPixel(GetClientWindowHeight()) - m_vertCoordFreq.Transform2fPixelPos(freq);
}

fPixel SignalControl::getPixYcurr(mV const curr) const
{
	return Convert2fPixel(GetClientWindowHeight()) - m_vertCoordCurr.Transform2fPixelPos(curr);
}

fPixel SignalControl::getPixXmax() const
{
	return getPixX(m_signalGenerator.TimeMax());
}

fPixel SignalControl::getPixYmaxFreq() const
{
	return getPixYfreq(m_signalGenerator.FreqMax() + m_signalGenerator.FreqBase());
}

fPixel SignalControl::getPixYmaxCurr() const
{
	return getPixYcurr(m_signalGenerator.CurrMax());
}

fPixel SignalControl::getPixYbase() const
{
	return getPixYfreq(m_signalGenerator.FreqBase());
}

fPixelPoint SignalControl::getPixPntFreq(fMicroSecs const time, fHertz const freq) const
{
	return fPixelPoint(getPixX(time), getPixYfreq(freq));
}

fPixelPoint SignalControl::getPixPntCurr(fMicroSecs const time, mV const curr) const
{
	return fPixelPoint(getPixX(time), getPixYcurr(curr));
}

fPixelPoint SignalControl::getPixPntMaxFreq() const
{
	return fPixelPoint(getPixXmax(), getPixYmaxFreq());
}

fPixelPoint SignalControl::getPixPntMaxCurr() const
{
	return fPixelPoint(getPixXmax(), getPixYmaxCurr());
}

fPixelPoint SignalControl::getGraphPntFreq(fMicroSecs const time) const
{
	return getPixPntFreq(time, m_signalGenerator.GetFrequency(time));
}

fPixelPoint SignalControl::getGraphPntCurr(fMicroSecs const time) const
{
	return getPixPntCurr(time, m_signalGenerator.GetPeakCurr(time));
}

void SignalControl::paintFreqCurve(D2D1::ColorF const col) const
{
	fMicroSecs const usResolution { m_signalGenerator.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
	fMicroSecs const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs const timeStart    { 0.0_MicroSecs };
	fMicroSecs const timeCutoff   { m_signalGenerator.CutoffTime() };
	fMicroSecs       timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint      prevPoint    { getGraphPntFreq(timeStart) };

	if (timeCutoff < timeEnd)
		timeEnd = timeCutoff;

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getGraphPntFreq(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, col);
		m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, col);
		prevPoint = actPoint;
	}
}

void SignalControl::paintCurrCurve(D2D1::ColorF const col) const
{
	fMicroSecs const usResolution { m_signalGenerator.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
	fMicroSecs const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs const timeStart    { 0.0_MicroSecs };
	fMicroSecs const timeCutoff   { m_signalGenerator.CutoffTime() };
	fMicroSecs       timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint      prevPoint    { getGraphPntCurr(timeStart) };

	if (timeCutoff < timeEnd)
		timeEnd = timeCutoff;

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getGraphPntCurr(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, col);
		m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, col);
		prevPoint = actPoint;
	}
}

void SignalControl::highlightMovedObject() const
{
	static D2D1::ColorF const COL_HI        { D2D1::ColorF::Red   };
	static fPixel       const LINE_WIDTH_HI { 3.0_fPixel };
	switch (m_moveMode)
	{
	case tMoveMode::MAX_FREQ_PNT:
		displayFreqMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI, DIAMOND_SIZE * 1.5f);
		displayTimeLine    (m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tMoveMode::MAX_FREQ:
		displayFreqLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tMoveMode::MAX_TIME:
		displayTimeLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tMoveMode::BASE_FREQ:
		displayBaseFrequency(COL_HI, LINE_WIDTH_HI);
		break;
	case tMoveMode::MAX_CURR:
		displayCurrLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tMoveMode::MAX_CURR_PNT:
		displayCurrMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI, DIAMOND_SIZE * 1.5f);
		displayTimeLine    (m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	default:
		break;
	}
}

void SignalControl::DoPaint()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

	if (m_computeThread.IsRunning())
	{
		if (m_signalGenerator.IsTriggerActive())
		{
			displayFreqMaxPoint(m_signalGenerator.TimeTilTrigger(), LINE_WIDTH_STD, m_colorFreq, DIAMOND_SIZE);
			displayCurrMaxPoint(m_signalGenerator.TimeTilTrigger(), LINE_WIDTH_STD, m_colorCurr, DIAMOND_SIZE);
			displayTimeLine    (m_signalGenerator.TimeTilTrigger(), LINE_WIDTH_STD, D2D1::ColorF::Black);
		}
	}
	else 
	{
		if (m_moveMode != tMoveMode::NONE)
			highlightMovedObject();
		displayFreqMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_STD, m_colorFreq, DIAMOND_SIZE);
		displayCurrMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_STD, m_colorCurr, DIAMOND_SIZE);
		displayTimeLine    (m_signalGenerator.TimeMax(), LINE_WIDTH_STD, D2D1::ColorF::Black);
	}
	displayBaseFrequency(m_colorFreq, LINE_WIDTH_STD);
	paintFreqCurve(m_colorFreq);
	paintCurrCurve(m_colorCurr);
}

void SignalControl::displayFreqMaxPoint
(
	fMicroSecs   const time, 
	fPixel       const wLine,
	D2D1::ColorF const col,
	fPixel       const wDiamond
) const
{
	m_upGraphics->FillDiamond(getGraphPntFreq(time), wDiamond, col);
	displayFreqLine(time, wLine, col);
}

void SignalControl::displayCurrMaxPoint
(
	fMicroSecs   const time, 
	fPixel       const wLine,
	D2D1::ColorF const col,
	fPixel       const wDiamond
) const
{
	m_upGraphics->FillDiamond(getGraphPntCurr(time), wDiamond, col);
	displayCurrLine(time, wLine, col);
}

void SignalControl::displayBaseFrequency
(
	D2D1::ColorF const colF,
	fPixel       const width
) 
const
{		
	fMicroSecs  const timeStart    { 0.0_MicroSecs };
	fMicroSecs  const timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fHertz      const freq         { m_signalGenerator.FreqBase() };
	fPixelPoint const fPixPntStart { getPixPntFreq(timeStart, freq) };
	fPixelPoint const fPixPntEnd   { getPixPntFreq(timeEnd,   freq) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
}

void SignalControl::displayFreqLine
(
	fMicroSecs   const time, 
	fPixel       const width, 
	D2D1::ColorF const colF
) const
{
	fHertz      const freq         { m_signalGenerator.GetFrequency(time) };
	fPixelPoint const fPixPntStart { getPixPntFreq(time, freq) };
	fPixelPoint const fPixPntEnd   { getPixPntFreq(0.0_MicroSecs, freq) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
}

void SignalControl::displayCurrLine
(
	fMicroSecs   const time, 
	fPixel       const width, 
	D2D1::ColorF const colF
) const
{
	fMicroSecs  const timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	mV          const currPeak     { m_signalGenerator.GetPeakCurr(time) };
	fPixelPoint const fPixPntStart { getPixPntCurr(time,    currPeak) };
	fPixelPoint const fPixPntEnd   { getPixPntCurr(timeEnd, currPeak) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
}

void SignalControl::displayTimeLine
(
	fMicroSecs   const time, 
	fPixel       const width, 
	D2D1::ColorF const col
) const
{
	fPixel      const fPixFreq     { getPixYfreq(m_signalGenerator.GetFrequency(time)) };
	fPixel      const fPixCurr     { getPixYcurr(m_signalGenerator.GetPeakCurr (time)) };
	fPixel      const fPixEnd      { min(fPixFreq, fPixCurr) };
	fPixel      const fPixX        { getPixX(time) };
	fPixel      const fPixStart    { getPixYfreq(0.0_fHertz) };
	fPixelPoint const fPixPntStart { fPixelPoint(fPixX, fPixStart) };
	fPixelPoint const fPixPntEnd   { fPixelPoint(fPixX, fPixEnd) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, col);
}

bool SignalControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);

	m_fPixGraphWidth = Convert2fPixel(width);

	m_horzCoord.SetOffset(0.0_fPixel);
	m_vertCoordFreq.SetOffset(0.0_fPixel);

	Trigger();  // cause repaint
	return true;
}

bool SignalControl::timeMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetX(), getPixXmax()) <= RECOG_DIST)
		&& IsInRange(pos.GetY(), getPixYmaxFreq(), getPixYfreq(0.0_fHertz));
}

bool SignalControl::freqMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYmaxFreq()) <= RECOG_DIST)
		&& (pos.GetX() <= getPixXmax());
}

bool SignalControl::currMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYmaxCurr()) <= RECOG_DIST)
		&& (pos.GetX() >= getPixXmax());
}

bool SignalControl::baseLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYbase()) <= RECOG_DIST)
		&& IsInRange(pos.GetX(), getPixX(0.0_MicroSecs), m_fPixGraphWidth);
}

void SignalControl::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	if (wParam & MK_LBUTTON)
	{
		fMicroSecs const usNew   { getTime(fPixCrsrPos.GetX()) };
		fHertz     const freqCrsr{ getFreq(fPixCrsrPos.GetY()) };
		fHertz     const freqNew { freqCrsr - m_signalGenerator.FreqBase() };
		mV         const currNew { getCurr(fPixCrsrPos.GetY()) };
		m_sigGenNew.SetParams(m_signalGenerator);
		switch (m_moveMode)
		{
		case tMoveMode::MAX_FREQ_PNT:
			m_sigGenNew.SetFreqMax(freqNew);
			m_sigGenNew.SetTimeMax(usNew);
			break;
		case tMoveMode::MAX_FREQ:
			m_sigGenNew.SetFreqMax(freqNew);
			break;
		case tMoveMode::MAX_TIME:
			m_sigGenNew.SetTimeMax(usNew);
			break;
		case tMoveMode::BASE_FREQ:
			m_sigGenNew.SetFreqBase(freqCrsr);
			break;
		case tMoveMode::MAX_CURR_PNT:
			m_sigGenNew.SetPeakVoltage(currNew);
			m_sigGenNew.SetTimeMax(usNew);
			break;
		case tMoveMode::MAX_CURR:
			m_sigGenNew.SetPeakVoltage(currNew);
			break;
		default:
			break;
		}
		m_commands.SetStimulusParams(m_signalGenerator, m_sigGenNew);
	}
	else  // left button not pressed: select
	{
		if (Distance(fPixCrsrPos, getPixPntMaxFreq()) <= 20.0_fPixel) 
			m_moveMode = tMoveMode::MAX_FREQ_PNT;
		else if (Distance(fPixCrsrPos, getPixPntMaxCurr()) <= 20.0_fPixel) 
			m_moveMode = tMoveMode::MAX_CURR_PNT;
		else if (baseLineSelected(fPixCrsrPos))
			m_moveMode = tMoveMode::BASE_FREQ;
		else if (freqMaxLineSelected(fPixCrsrPos))
			m_moveMode = tMoveMode::MAX_FREQ;
		else if (currMaxLineSelected(fPixCrsrPos))
			m_moveMode = tMoveMode::MAX_CURR;
		else if (timeMaxLineSelected(fPixCrsrPos))
 			m_moveMode = tMoveMode::MAX_TIME;
		else
			m_moveMode = tMoveMode::NONE;
		Trigger();   // cause repaint
	}
	//TrackMouse();
}

bool SignalControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	m_moveMode = tMoveMode::NONE;
	Trigger();   // cause repaint
	return false;
}
