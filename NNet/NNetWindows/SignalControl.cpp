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

SignalControl::~SignalControl()
{
	m_signalGenerator.UnregisterObserver(this);
}

SignalControl::SignalControl
(
	HWND             const   hwndParent,
	ComputeThread    const & computeThread,
	SignalGenerator        & sigGen,
	PixCoordFp<fMicroSecs> * pHorzCoord,
	PixCoordFp<fHertz>     * pVertCoord
)
  : m_computeThread(computeThread),
	m_signalGenerator(sigGen),
	m_pHorzCoord(pHorzCoord),
	m_pVertCoord(pVertCoord)
{
	m_signalGenerator.RegisterObserver(this);

	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		L"ClassSignalControl",
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		nullptr,
		nullptr
	);

	m_graphics.Initialize(hwnd);
	m_trackStruct.hwndTrack = hwnd;
}

void SignalControl::Reset()
{
	m_trackStruct.hwndTrack = HWND(0);
	(void)TrackMouseEvent(& m_trackStruct);
}

void SignalControl::Stop()
{
	Reset();
	m_graphics.ShutDown();
	DestroyWindow();
}

fMicroSecs const SignalControl::getTime(fPixel const fPix)
{
	return m_pHorzCoord->Transform2logUnitPos(fPix);
}

fHertz const SignalControl::getFreq(fPixel const fPix)
{
	return m_pVertCoord->Transform2logUnitPos(Convert2fPixel(GetClientWindowHeight()) - fPix);
}

fPixel const SignalControl::getPixX(fMicroSecs const time) const
{
	return fPixel(m_pHorzCoord->Transform2fPixelPos(time));
}

fPixel const SignalControl::getPixY(fHertz const freq) const
{
	return Convert2fPixel(GetClientWindowHeight()) - fPixel(m_pVertCoord->Transform2fPixelPos(freq));
}

fPixel const SignalControl::getPixXmax() const
{
	return getPixX(m_signalGenerator.TimeMax());
}

fPixel const SignalControl::getPixYmax() const
{
	return getPixY(m_signalGenerator.FreqMax());
}

fPixel const SignalControl::getPixYbase() const
{
	return getPixY(m_signalGenerator.FreqBase());
}

fPixelPoint const SignalControl::getPixPnt(fMicroSecs const time, fHertz const freq) const
{
	return fPixelPoint(getPixX(time), getPixY(freq));
}

fPixelPoint const SignalControl::getPixPntMax() const
{
	return fPixelPoint(getPixXmax(), getPixYmax());
}

fPixelPoint const SignalControl::getGraphPnt(fMicroSecs const time) const
{
	return getPixPnt(time, m_signalGenerator.GetFrequency(time));
}

void SignalControl::doPaint() const
{
	fPixelRect fPixRect = Convert2fPixelRect(GetClPixelRect());
	m_graphics.FillRectangle(fPixRect, D2D1::ColorF::Ivory);
//	m_graphics.DrawRectangle(fPixRect, D2D1::ColorF::Black, 1.0_fPixel);

	D2D1::ColorF const color        { D2D1::ColorF::Black };
	fMicroSecs   const usResolution { m_signalGenerator.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
	fMicroSecs   const usPixelSize  { m_pHorzCoord->GetPixelSize() };
	fMicroSecs   const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs   const timeStart    { 0.0_MicroSecs };
	fMicroSecs   const timeCutoff   { m_signalGenerator.CutoffTime() };
	fMicroSecs         timeEnd      { m_pHorzCoord->Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint        prevPoint    { getGraphPnt(timeStart) };

	if (timeCutoff < timeEnd)
		timeEnd = timeCutoff;

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getGraphPnt(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_graphics.DrawLine(prevPoint, stepPoint, m_fPixLineWidth, color);
		m_graphics.DrawLine(stepPoint, actPoint,  m_fPixLineWidth, color);
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
	m_graphics.FillDiamond(getGraphPnt(time), wDiamond, col);
	displayFreqLine(time, wLine, col);
	displayTimeLine(time, wLine, col);
}

void SignalControl::displayBaseFrequency
(
	fPixel       const w, 
	D2D1::ColorF const colF
) 
const
{		
	fMicroSecs   const timeStart { 0.0_MicroSecs };
	fMicroSecs   const timeEnd   { m_pHorzCoord->Transform2logUnitSize(m_fPixGraphWidth) };
	m_graphics.DrawLine
	(
		getPixPnt(timeStart, m_signalGenerator.FreqBase()), 
		getPixPnt(timeEnd,   m_signalGenerator.FreqBase()), 
		w,
		colF
	);
}

void SignalControl::displayFreqLine(fMicroSecs const time, fPixel const w, D2D1::ColorF const colF) const
{
	m_graphics.DrawLine(getGraphPnt(time), getPixPnt(0.0_MicroSecs, m_signalGenerator.GetFrequency(time)), w, colF);
}

void SignalControl::displayTimeLine(fMicroSecs const time, fPixel const w, D2D1::ColorF const colF) const
{
	m_graphics.DrawLine(getGraphPnt(time), getPixPnt(time, 0.0_fHertz),  w, colF);
}

void SignalControl::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint(&ps);
		if (m_graphics.StartFrame(hDC))
		{
			doPaint();
			m_graphics.EndFrame();
		}
		EndPaint(&ps);
	}
}

bool SignalControl::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);

	m_graphics.Resize(width, height);

	fPixel fPixWinWidth  { Convert2fPixel(PIXEL(width )) };
	fPixel fPixWinHeight { Convert2fPixel(PIXEL(height)) };

	m_fPixGraphWidth = fPixWinWidth;

	m_pHorzCoord->SetOffset(0.0_fPixel);
	m_pVertCoord->SetOffset(0.0_fPixel);

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
		fMicroSecs usCrsr   { getTime(fPixCrsrPos.GetX()) };
		fHertz     freqCrsr { getFreq(fPixCrsrPos.GetY()) };
		if (freqCrsr < m_signalGenerator.FreqBase())
			freqCrsr = m_signalGenerator.FreqBase();

		switch (m_trackMode)
		{
		case tTrackMode::MAX_PNT:
			m_signalGenerator.SetFreqMax(freqCrsr);
			m_signalGenerator.SetTimeMax(usCrsr);
			break;
		case tTrackMode::MAX_FREQ:
			m_signalGenerator.SetFreqMax(freqCrsr);
			break;
		case tTrackMode::MAX_TIME:
			m_signalGenerator.SetTimeMax(usCrsr);
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
	(void)TrackMouseEvent(& m_trackStruct);
}

bool SignalControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	m_trackMode = tTrackMode::NONE;
	Trigger();   // cause repaint
	return false;
}

bool SignalControl::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	WORD const wId = LOWORD(wParam);
	switch (wId)
	{
		//case IDD_APPLY_PARAMETERS:
		//	m_signalGenerator.SetParameterValues();
		//	return true;

	case IDD_RESET_PARAMETERS:
		m_signalGenerator.LoadParameterValues();
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
