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
#include "NNetModelCommands.h"
#include "SignalControl.h"

SignalControl::~SignalControl()
{
	m_signalGenerator.UnregisterObserver(*this);
}

SignalControl::SignalControl
(
	HWND                 const   hwndParent,
	ComputeThread        const & computeThread,
	SignalGenerator            & sigGen,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<fHertz>     & vertCoord,
	NNetModelCommands          & commands
)
  : m_commands(commands),
	m_computeThread(computeThread),
	m_signalGenerator(sigGen),
	m_horzCoord(horzCoord),
	m_vertCoord(vertCoord)
{
	m_signalGenerator.RegisterObserver(*this);
	m_horzCoord.RegisterObserver(*this);
	m_vertCoord.RegisterObserver(*this);
	GraphicsWindow::Initialize(hwndParent, L"ClassSignalControl", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE);
}

fMicroSecs SignalControl::getTime(fPixel const fPix) const
{
	return m_horzCoord.Transform2logUnitPos(fPix);
}

fHertz SignalControl::getFreq(fPixel const fPix) const 
{
	return m_vertCoord.Transform2logUnitPos(Convert2fPixel(GetClientWindowHeight()) - fPix);
}

fPixel SignalControl::getPixX(fMicroSecs const time) const
{
	return fPixel(m_horzCoord.Transform2fPixelPos(time));
}

fPixel SignalControl::getPixY(fHertz const freq) const
{
	return Convert2fPixel(GetClientWindowHeight()) - fPixel(m_vertCoord.Transform2fPixelPos(freq));
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

void SignalControl::DoPaint()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

	D2D1::ColorF const color        { D2D1::ColorF::Black };
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
		m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, color);
		m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, color);
		prevPoint = actPoint;
	}

	switch (m_trackMode)
	{
	case tTrackMode::MAX_PNT:
		displayPoint(m_signalGenerator.TimeMax(), DIAMOND_SIZE * 1.5f, LINE_WIDTH_HI, COL_HI);
		break;
	case tTrackMode::MAX_FREQ:
		displayFreqLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tTrackMode::MAX_TIME:
		displayTimeLine(m_signalGenerator.TimeMax(), LINE_WIDTH_HI, COL_HI);
		break;
	case tTrackMode::BASE_FREQ:
		displayBaseFrequency(LINE_WIDTH_HI, COL_HI);
		break;
	default:
		break;
	}

	if (m_computeThread.IsRunning() && m_signalGenerator.IsTriggerActive())
	{
		displayPoint(m_signalGenerator.TimeTilTrigger(), DIAMOND_SIZE, LINE_WIDTH_STD, COL_STD);
	}
	else 
	{
		displayPoint(m_signalGenerator.TimeMax(), DIAMOND_SIZE, LINE_WIDTH_STD, COL_STD);
		displayBaseFrequency(LINE_WIDTH_STD, COL_STD);
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
	fPixel       const width, 
	D2D1::ColorF const colF
) 
const
{		
	fMicroSecs   const timeStart { 0.0_MicroSecs };
	fMicroSecs   const timeEnd   { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	m_upGraphics->DrawLine
	(
		getPixPnt(timeStart, m_signalGenerator.FreqBase()), 
		getPixPnt(timeEnd,   m_signalGenerator.FreqBase()), 
		width,
		colF
	);
}

void SignalControl::displayFreqLine(fMicroSecs const time, fPixel const w, D2D1::ColorF const colF) const
{
	m_upGraphics->DrawLine(getGraphPnt(time), getPixPnt(0.0_MicroSecs, m_signalGenerator.GetFrequency(time)), w, colF);
}

void SignalControl::displayTimeLine(fMicroSecs const time, fPixel const w, D2D1::ColorF const colF) const
{
	m_upGraphics->DrawLine(getGraphPnt(time), getPixPnt(time, 0.0_fHertz),  w, colF);
}

bool SignalControl::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	GraphicsWindow::OnSize(wParam, lParam);

	m_fPixGraphWidth = Convert2fPixel(PIXEL(LOWORD(lParam)));

	m_horzCoord.SetOffset(0.0_fPixel);
	m_vertCoord.SetOffset(0.0_fPixel);

	Trigger();  // cause repaint
	return true;
}

bool SignalControl::timeMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetX(), getPixXmax()) <= 10.0_fPixel)
		&& IsInRange(pos.GetY(), getPixYmax(), getPixY(0.0_fHertz));
}

bool SignalControl::freqMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYmax()) <= 10.0_fPixel)
		&& IsInRange(pos.GetX(), getPixX(0.0_MicroSecs), getPixXmax());
}

bool SignalControl::baseLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), getPixYbase()) <= 10.0_fPixel)
		&& IsInRange(pos.GetX(), getPixX(0.0_MicroSecs), m_fPixGraphWidth);
}

void SignalControl::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	if (wParam & MK_LBUTTON)
	{
		fMicroSecs const usCrsr   { getTime(fPixCrsrPos.GetX()) };
		fHertz     const freqCrsr { getFreq(fPixCrsrPos.GetY()) };
		fHertz           freqLim  { freqCrsr - m_signalGenerator.FreqBase() };
		fMicroSecs       usLim    { usCrsr };
		if (freqLim < 0._fHertz)
			freqLim = 0._fHertz;
		if (usLim <= 1._MicroSecs)
			usLim = 1._MicroSecs;
		switch (m_trackMode)
		{
		case tTrackMode::MAX_PNT:
			m_commands.SetStimulusParams(m_signalGenerator, fMicroSecs::NULL_VAL(), freqLim);
			m_commands.SetStimulusParams(m_signalGenerator, usLim, fHertz::NULL_VAL());
			break;
		case tTrackMode::MAX_FREQ:
			m_commands.SetStimulusParams(m_signalGenerator, fMicroSecs::NULL_VAL(), freqLim);
			break;
		case tTrackMode::MAX_TIME:
			m_commands.SetStimulusParams(m_signalGenerator, usLim, fHertz::NULL_VAL());
			break;
		case tTrackMode::BASE_FREQ:
			m_signalGenerator.SetFreqBase(freqCrsr);
			break;
		default:
			break;
		}
	}
	else  // left button not pressed: select
	{
		if (Distance(fPixCrsrPos, getPixPntMax()) <= 20.0_fPixel) 
			m_trackMode = tTrackMode::MAX_PNT;
		else if (baseLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::BASE_FREQ;
		else if (freqMaxLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::MAX_FREQ;
		else if (timeMaxLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::MAX_TIME;
		else
			m_trackMode = tTrackMode::NONE;
	}
	Trigger();   // cause repaint
	TrackMouse();
}

bool SignalControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	m_trackMode = tTrackMode::NONE;
	Trigger();   // cause repaint
	return false;
}

bool SignalControl::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDD_RESET_PARAMETERS:
		m_signalGenerator.LoadParameterValues();
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
