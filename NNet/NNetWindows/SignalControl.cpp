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
	PixFpDimension<mV>         & vertCoordVolt
)
  :	m_horzCoord(horzCoord),
	m_vertCoordFreq(vertCoordFreq),
	m_vertCoordVolt(vertCoordVolt),
	m_computeThread(computeThread),
	m_signalGenerator(signalGenerator),
	m_commands(commands)
{
	m_signalGenerator.RegisterObserver(*this); // signal generator data can be changed fron outside
	m_horzCoord      .RegisterObserver(*this); 
	m_vertCoordFreq  .RegisterObserver(*this);
	m_vertCoordVolt  .RegisterObserver(*this);
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

mV SignalControl::getVolt(fPixel const fPix) const 
{
	return m_vertCoordVolt.Transform2logUnitPos(Convert2fPixel(GetClientWindowHeight()) - fPix);
}

fPixel SignalControl::getPixX(fMicroSecs const time) const
{
	return fPixel(m_horzCoord.Transform2fPixelPos(time));
}

fPixel SignalControl::getPixYfreq(fHertz const freq) const
{
	return Convert2fPixel(GetClientWindowHeight()) - m_vertCoordFreq.Transform2fPixelPos(freq);
}

fPixel SignalControl::getPixYvolt(mV const voltage) const
{
	return Convert2fPixel(GetClientWindowHeight()) - m_vertCoordVolt.Transform2fPixelPos(voltage);
}

fPixel SignalControl::getPixXmax() const
{
	return getPixX(m_signalGenerator.TimeMax());
}

fPixel SignalControl::getPixYmaxFreq() const
{
	return getPixYfreq(m_signalGenerator.FreqMax() + m_signalGenerator.FreqBase());
}

fPixel SignalControl::getPixYmaxVolt() const
{
	return getPixYvolt(m_signalGenerator.VoltMax());
}

fPixel SignalControl::getPixYbase() const
{
	return getPixYfreq(m_signalGenerator.FreqBase());
}

fPixelPoint SignalControl::getPixPntFreq(fMicroSecs const time, fHertz const freq) const
{
	return fPixelPoint(getPixX(time), getPixYfreq(freq));
}

fPixelPoint SignalControl::getPixPntVolt(fMicroSecs const time, mV const voltage) const
{
	return fPixelPoint(getPixX(time), getPixYvolt(voltage));
}

fPixelPoint SignalControl::getPixPntMaxFreq() const
{
	return fPixelPoint(getPixXmax(), getPixYmaxFreq());
}

fPixelPoint SignalControl::getPixPntMaxVolt() const
{
	return fPixelPoint(getPixXmax(), getPixYmaxVolt());
}

fPixelPoint SignalControl::getGraphPntFreq(fMicroSecs const time) const
{
	return getPixPntFreq(time, m_signalGenerator.GetFrequency(time));
}

fPixelPoint SignalControl::getGraphPntVolt(fMicroSecs const time) const
{
	return getPixPntVolt(time, m_signalGenerator.GetVoltage(time));
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
		displayVoltLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tMoveMode::MAX_CURR_PNT:
		displayVoltMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI, DIAMOND_SIZE * 1.5f);
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
			displayVoltMaxPoint(m_signalGenerator.TimeTilTrigger(), LINE_WIDTH_STD, m_colorVolt, DIAMOND_SIZE);
			displayTimeLine    (m_signalGenerator.TimeTilTrigger(), LINE_WIDTH_STD, D2D1::ColorF::Black);
		}
	}
	else 
	{
		if (m_moveMode != tMoveMode::NONE)
			highlightMovedObject();
		displayFreqMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_STD, m_colorFreq, DIAMOND_SIZE);
		displayVoltMaxPoint(m_signalGenerator.TimeMax(), LINE_WIDTH_STD, m_colorVolt, DIAMOND_SIZE);
		displayTimeLine    (m_signalGenerator.TimeMax(), LINE_WIDTH_STD, D2D1::ColorF::Black);
	}
	displayBaseFrequency(m_colorFreq, LINE_WIDTH_STD);
	paintCurve([this](fMicroSecs const t){ return getGraphPntFreq(t); }, m_colorFreq );
	paintCurve([this](fMicroSecs const t){ return getGraphPntVolt(t); }, m_colorVolt );
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

void SignalControl::displayVoltMaxPoint
(
	fMicroSecs   const time, 
	fPixel       const wLine,
	D2D1::ColorF const col,
	fPixel       const wDiamond
) const
{
	m_upGraphics->FillDiamond(getGraphPntVolt(time), wDiamond, col);
	displayVoltLine(time, wLine, col);
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
	fPixelPoint const fPixPntStart { getPixPntFreq(time,          freq) };
	fPixelPoint const fPixPntEnd   { getPixPntFreq(0.0_MicroSecs, freq) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
}

void SignalControl::displayVoltLine
(
	fMicroSecs   const time, 
	fPixel       const width, 
	D2D1::ColorF const colF
) const
{
	fMicroSecs  const timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	mV          const voltPeak     { m_signalGenerator.GetVoltage(time) };
	fPixelPoint const fPixPntStart { getPixPntVolt(time,    voltPeak) };
	fPixelPoint const fPixPntEnd   { getPixPntVolt(timeEnd, voltPeak) };
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
	fPixel      const fPixVolt     { getPixYvolt(m_signalGenerator.GetVoltage (time)) };
	fPixel      const fPixEnd      { min(fPixFreq, fPixVolt) };
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
	return (Distance(pos.GetY(), getPixYmaxVolt()) <= RECOG_DIST)
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
		mV         const currNew { getVolt(fPixCrsrPos.GetY()) };
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
		else if (Distance(fPixCrsrPos, getPixPntMaxVolt()) <= 20.0_fPixel) 
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
