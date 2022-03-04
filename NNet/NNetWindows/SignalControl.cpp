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
	m_signalGenerator.RegisterObserver(*this);
	m_horzCoord.RegisterObserver(*this);
	m_vertCoordFreq.RegisterObserver(*this);
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

fPixel SignalControl::getPixX(fMicroSecs const time) const
{
	return fPixel(m_horzCoord.Transform2fPixelPos(time));
}

fPixel SignalControl::getPixY(fHertz const freq) const
{
	return Convert2fPixel(GetClientWindowHeight()) - fPixel(m_vertCoordFreq.Transform2fPixelPos(freq));
}

fPixel SignalControl::getPixXmax() const
{
	return getPixX(m_signalGenerator.TimeMax());
}

fPixel SignalControl::getPixYmax() const
{
	return getPixY(m_signalGenerator.FreqMax() + m_signalGenerator.FreqBase());
}

fPixel SignalControl::getPixYbase() const
{
	return getPixY(m_signalGenerator.FreqBase());
}

fPixelPoint SignalControl::getPixPnt(fMicroSecs const time, fHertz const freq) const
{
	return fPixelPoint(getPixX(time), getPixY(freq));
}

fPixelPoint SignalControl::getPixPntMax() const
{
	return fPixelPoint(getPixXmax(), getPixYmax());
}

fPixelPoint SignalControl::getGraphPnt(fMicroSecs const time) const
{
	return getPixPnt(time, m_signalGenerator.GetFrequency(time));
}

void SignalControl::paintSigGenData(D2D1::ColorF const col) const
{
	fMicroSecs   const usResolution { m_signalGenerator.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
	fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs   const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs   const timeStart    { 0.0_MicroSecs };
	fMicroSecs   const timeCutoff   { m_signalGenerator.CutoffTime() };
	fMicroSecs         timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint        prevPoint    { getGraphPnt(timeStart) };

	if (timeCutoff < timeEnd)
		timeEnd = timeCutoff;

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getGraphPnt(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, col);
		m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, col);
		prevPoint = actPoint;
	}

	displayBaseFrequency(col, LINE_WIDTH_STD);
}

void SignalControl::highlightMovedObject() const
{
	static D2D1::ColorF const COL_HI        { D2D1::ColorF::Red   };
	static fPixel       const LINE_WIDTH_HI { 3.0_fPixel };
	switch (m_moveMode)
	{
	case tMoveMode::MAX_PNT:
		displayPoint(m_signalGenerator.TimeMax(), DIAMOND_SIZE * 1.5f, LINE_WIDTH_HI, COL_HI);
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
			displayPoint(m_signalGenerator.TimeTilTrigger(), DIAMOND_SIZE, LINE_WIDTH_STD, m_colorFreq);
	}
	else 
	{
		if (m_moveMode != tMoveMode::NONE)
			highlightMovedObject();
		paintSigGenData(m_colorFreq);
		displayPoint(m_signalGenerator.TimeMax(), DIAMOND_SIZE, LINE_WIDTH_STD, m_colorFreq);
	}
}

void SignalControl::displayPoint
(
	fMicroSecs   const time, 
	fPixel       const wDiamond, 
	fPixel       const wLine,
	D2D1::ColorF const col
) const
{
	m_upGraphics->FillDiamond(getGraphPnt(time), wDiamond, col);
	displayFreqLine(time, wLine, col);
	displayTimeLine(time, wLine, col);
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
	fPixelPoint const fPixPntStart { getPixPnt(timeStart, freq) };
	fPixelPoint const fPixPntEnd   { getPixPnt(timeEnd,   freq) };
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
	fPixelPoint const fPixPntStart { getPixPnt(time, freq) };
	fPixelPoint const fPixPntEnd   { getPixPnt(0.0_MicroSecs, freq) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
}

void SignalControl::displayTimeLine
(
	fMicroSecs   const time, 
	fPixel       const width, 
	D2D1::ColorF const colF
) const
{
	fHertz      const freq         { m_signalGenerator.GetFrequency(time) };
	fPixelPoint const fPixPntStart { getPixPnt(time, freq) };
	fPixelPoint const fPixPntEnd   { getPixPnt(time, 0.0_fHertz) };
	m_upGraphics->DrawLine(fPixPntStart, fPixPntEnd, width, colF);
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
		&& IsInRange(pos.GetY(), getPixYmax(), getPixY(0.0_fHertz));
}

bool SignalControl::freqMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYmax()) <= RECOG_DIST)
		&& IsInRange(pos.GetX(), getPixX(0.0_MicroSecs), getPixXmax());
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
		fMicroSecs const usLim    { getTime(fPixCrsrPos.GetX()) };
		fHertz     const freqCrsr { getFreq(fPixCrsrPos.GetY()) };
		fHertz           freqLim  { freqCrsr - m_signalGenerator.FreqBase() };
		m_sigGenNew.SetParams(m_signalGenerator);
		switch (m_moveMode)
		{
		case tMoveMode::MAX_PNT:
			m_sigGenNew.SetFreqMax(freqLim);
			m_sigGenNew.SetTimeMax(usLim);
			break;
		case tMoveMode::MAX_FREQ:
			m_sigGenNew.SetFreqMax(freqLim);
			break;
		case tMoveMode::MAX_TIME:
			m_sigGenNew.SetTimeMax(usLim);
			break;
		case tMoveMode::BASE_FREQ:
			m_sigGenNew.SetFreqBase(freqCrsr);
			break;
		default:
			break;
		}
		m_commands.SetStimulusParams(m_signalGenerator, m_sigGenNew);
	}
	else  // left button not pressed: select
	{
		if (Distance(fPixCrsrPos, getPixPntMax()) <= 20.0_fPixel) 
			m_moveMode = tMoveMode::MAX_PNT;
		else if (baseLineSelected(fPixCrsrPos))
			m_moveMode = tMoveMode::BASE_FREQ;
		else if (freqMaxLineSelected(fPixCrsrPos))
			m_moveMode = tMoveMode::MAX_FREQ;
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
