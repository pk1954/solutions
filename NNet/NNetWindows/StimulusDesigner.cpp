// StimulusDesigner.cpp
//
// NNetWindows

#include <math.h>       // pow
#include "stdafx.h"
#include "PointType.h"
#include "NNetModelReaderInterface.h"
#include "SignalGenerator.h"
#include "NNetParameters.h"
#include "StimulusDesigner.h"

void StimulusDesigner::Start
(
	HWND                     const   hwndParent,
	SignalGenerator        * const   pSignalGenerator,
	NNetModelReaderInterface const & nmri
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		L"ClassMonitorWindow",
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		nullptr,
		nullptr
	);
	m_graphics.Initialize(hwnd);
	SetWindowText(hwnd, L"StimulusDesigner");
	m_trackStruct.hwndTrack = hwnd;

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(1000._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);
	m_horzCoord.RegisterObserver(this);

	m_horzScale.InitHorzScale(&m_horzCoord, &m_graphics, L"s", 1e6f);
	m_horzScale.SetOrientation(false);
	m_horzScale.Recalc();

	m_vertCoord.SetPixelSize(0.25_fHertz);
	m_vertCoord.SetPixelSizeLimits(0.001_fHertz, 10._fHertz); 
	m_vertCoord.SetZoomFactor(1.3f);
	m_vertCoord.RegisterObserver(this);

	m_vertScale.InitVertScale(& m_vertCoord, & m_graphics, L"Hz", 1e0f);	
	m_vertScale.SetOrientation(true);
	m_vertScale.Recalc();

	m_pSignalGenerator      = pSignalGenerator;
	m_trackStruct.hwndTrack = hwnd;
	m_pNMRI                 = &nmri;
}

void StimulusDesigner::Reset()
{
	m_trackStruct.hwndTrack = HWND(0);
	(void)TrackMouseEvent(& m_trackStruct);
}

void StimulusDesigner::Stop()
{
	Reset();
	m_graphics.ShutDown();
	DestroyWindow();
}

fPixel const StimulusDesigner::getPixX(fMicroSecs const time) const
{
	return fPixel(m_horzCoord.Transform2fPixelPos(time));
}

fPixel const StimulusDesigner::getPixY(fHertz const freq) const
{
	return fPixel(m_vertCoord.Transform2fPixelPos(-freq));
}

fPixelPoint const StimulusDesigner::getPixPnt(fMicroSecs const time, fHertz const freq) const
{
	return fPixelPoint(getPixX(time), getPixY(freq));
}

fPixelPoint const StimulusDesigner::getGraphPnt(fMicroSecs const time) const
{
	fHertz const fHertzAct { m_pSignalGenerator->GetFrequency(time) };
	return getPixPnt(time, fHertzAct);
}

void StimulusDesigner::doPaint() const
{
	D2D1::ColorF const color        { D2D1::ColorF::Black };
	fMicroSecs   const usResolution { m_pNMRI->TimeResolution() };
	fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs   const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs   const timeStart    { 0.0_MicroSecs };
	fMicroSecs   const timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint        prevPoint    { getGraphPnt(timeStart) };

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getGraphPnt(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_graphics.DrawLine(prevPoint, stepPoint, m_fPixLineWidth, color);
		m_graphics.DrawLine(stepPoint, actPoint,  m_fPixLineWidth, color);
		prevPoint = actPoint;
	}

	if (m_zoomMode == tZoomMode::HORZ)
		m_graphics.FillRectangle(m_horzScale.GetScaleRect(), D2D1::ColorF::Aquamarine);
	else if (m_zoomMode == tZoomMode::VERT)
		m_graphics.FillRectangle(m_vertScale.GetScaleRect(), D2D1::ColorF::Aquamarine);

	m_horzScale.Display();
	m_vertScale.Display();

	switch (m_trackMode)
	{
	case tTrackMode::MAX_PNT:
		displayDiamondAtMaximum(true);
		break;
	case tTrackMode::MAX_FREQ:
		displayFreqMaxLine(true);
		break;
	case tTrackMode::MAX_TIME:
		displayTimeMaxLine(true);
		break;
	case tTrackMode::BASE_FREQ:
		displayBaseFrequency(timeStart, timeEnd, true);
		break;
	default:
		break;
	}
	displayDiamondAtMaximum(false);
	displayFreqMaxLine     (false);
	displayTimeMaxLine     (false);
	displayBaseFrequency   (timeStart, timeEnd, false);
}

void StimulusDesigner::displayBaseFrequency
(
	fMicroSecs const timeStart, 
	fMicroSecs const timeEnd,
	bool       const bEmphasized
) const
{		
	fPixel      const fPixBaseFreq(getPixY(m_pParameters->BaseFrequency()));
	fPixelPoint const fPixPntStart(getPixX(timeStart), fPixBaseFreq);
	fPixelPoint const fPixPointEnd(getPixX(timeEnd),   fPixBaseFreq);
	m_graphics.DrawLine
	(
		fPixPntStart, 
		fPixPointEnd, 
		bEmphasized ? 3.0_fPixel : 1.0_fPixel,
		bEmphasized ? D2D1::ColorF::Red : D2D1::ColorF::Green
	);
}

void StimulusDesigner::displayFreqMaxLine(bool const bEmphasized) const
{
	m_graphics.DrawLine
	(
		fPixelPoint(m_fPixPntMax.GetX(),          m_fPixPntMax.GetY()), 
		fPixelPoint(m_horzCoord.GetPixelOffset(), m_fPixPntMax.GetY()), 
		bEmphasized ? 3.0_fPixel : 1.0_fPixel,
		bEmphasized ? D2D1::ColorF::Red : D2D1::ColorF::Green
	);
}
void StimulusDesigner::displayTimeMaxLine(bool const bEmphasized) const
{
	m_graphics.DrawLine
	(
		fPixelPoint(m_fPixPntMax.GetX(), m_fPixPntMax.GetY()), 
		fPixelPoint(m_fPixPntMax.GetX(), m_vertCoord.GetPixelOffset()), 
		bEmphasized ? 3.0_fPixel : 1.0_fPixel,
		bEmphasized ? D2D1::ColorF::Red : D2D1::ColorF::Green
	);
}

void StimulusDesigner::displayDiamondAtMaximum(bool const bEmphasized) const
{
	m_graphics.FillDiamond
	(
		m_fPixPntMax, 
		bEmphasized ? DIAMOND_SIZE * 1.5f : DIAMOND_SIZE,
		bEmphasized ? D2D1::ColorF::Red : D2D1::ColorF::Lime
	);
}

void StimulusDesigner::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint(&ps);
		if (m_graphics.StartFrame(hDC))
		{
			m_fPixPntMax = getGraphPnt(m_pParameters->StimulusMaxTime());
			doPaint();
			m_graphics.EndFrame();
		}
		EndPaint(&ps);
	}
}

bool StimulusDesigner::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	static const float BORDER { 0.1f };
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize(width, height);
	fPixel fPixWinWidth  { Convert2fPixel(PIXEL(width )) };
	fPixel fPixWinHeight { Convert2fPixel(PIXEL(height)) };
	fPixel fPixXoffset = fPixWinWidth * BORDER;
	fPixel fPixYoffset = fPixWinHeight * (1.0f - BORDER);
	m_fPixGraphWidth = fPixWinWidth  * (1.0f - 2.0f * BORDER);
	m_horzCoord.SetOffset(fPixXoffset);
	m_vertCoord.SetOffset(fPixYoffset);
	m_horzScale.SetOrthoOffset(fPixYoffset);
	m_vertScale.SetOrthoOffset(fPixXoffset);
	m_horzScale.Recalc();
	m_vertScale.Recalc();
	Trigger();  // cause repaint
	return true;
}

bool StimulusDesigner::timeMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetX(), m_fPixPntMax.GetX()) <= 10.0_fPixel)
		&& (getPixY(0.0_fHertz) >= pos.GetY())
		&& (pos.GetY() >= m_fPixPntMax.GetY());
}

bool StimulusDesigner::freqMaxLineSelected(fPixelPoint const & pos) const
{
	return (Distance(pos.GetY(), m_fPixPntMax.GetY()) <= 10.0_fPixel)
		&& (getPixX(0.0_MicroSecs) <= pos.GetX())
		&& (pos.GetX() <= m_fPixPntMax.GetX());
}

bool StimulusDesigner::baseLineSelected(fPixelPoint const & pos) const
{
	fMicroSecs const timeEnd  { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixel     const fPixFreq { getPixY(m_pParameters->BaseFrequency()) };
	return (Distance(pos.GetY(), fPixFreq) <= 10.0_fPixel)
		&& (getPixX(0.0_MicroSecs) <= pos.GetX())
		&& (pos.GetX() <= getPixX(timeEnd));
}

bool StimulusDesigner::horzScaleSelected(fPixelPoint const & pos) const
{
	return m_horzScale.GetScaleRect().Includes(pos);
}

bool StimulusDesigner::vertScaleSelected(fPixelPoint const & pos) const
{
	return m_vertScale.GetScaleRect().Includes(pos);
}

void StimulusDesigner::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };
	fMicroSecs  const usPeakTime  { m_pParameters->StimulusMaxTime() };
	fHertz      const freqMax     { m_pSignalGenerator->GetFrequency(usPeakTime) };

	if (wParam & MK_LBUTTON)
	{
		fHertz const freqBase { m_pParameters->BaseFrequency() };
		fMicroSecs   usMax    {   m_horzCoord.Transform2logUnitPos(fPixCrsrPos.GetX()) };
		fHertz       freqMax  { - m_vertCoord.Transform2logUnitPos(fPixCrsrPos.GetY()) };

		freqMax -= freqBase;
		if (usMax < 0._MicroSecs)
			usMax = 0._MicroSecs;
		if (freqMax < 0._fHertz)
			freqMax = 0._fHertz;

		switch (m_trackMode)
		{
		case tTrackMode::MAX_PNT:
			m_pParameters->SetParameterValue(ParamType::Value::stimulusMaxFreq, freqMax.GetValue());
			m_pParameters->SetParameterValue(ParamType::Value::stimulusMaxTime, usMax.GetValue());
			break;
		case tTrackMode::MAX_FREQ:
			m_pParameters->SetParameterValue(ParamType::Value::stimulusMaxFreq, freqMax.GetValue());
			break;
		case tTrackMode::MAX_TIME:
			m_pParameters->SetParameterValue(ParamType::Value::stimulusMaxTime, usMax.GetValue());
			break;
		case tTrackMode::BASE_FREQ:
			m_pParameters->SetParameterValue(ParamType::Value::baseFrequency, freqMax.GetValue());
			break;
		default:
			break;
		}
		m_fPixPntMax = getGraphPnt(m_pParameters->StimulusMaxTime());
	}
	else  // left button not pressed: select
	{
		m_fPixPntMax = getGraphPnt(m_pParameters->StimulusMaxTime());
		if (Distance(fPixCrsrPos, m_fPixPntMax) <= 20.0_fPixel) 
			m_trackMode = tTrackMode::MAX_PNT;
		else if (baseLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::BASE_FREQ;
		else if (freqMaxLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::MAX_FREQ;
		else if (timeMaxLineSelected(fPixCrsrPos))
			m_trackMode = tTrackMode::MAX_TIME;
		else
			m_trackMode = tTrackMode::NONE;

		if (horzScaleSelected(fPixCrsrPos))
			m_zoomMode = tZoomMode::HORZ;
		else if (vertScaleSelected(fPixCrsrPos))
			m_zoomMode = tZoomMode::VERT;
		else
			m_zoomMode = tZoomMode::NONE;
	}
	Trigger();   // cause repaint
	(void)TrackMouseEvent(& m_trackStruct);
}

void StimulusDesigner::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	if (m_zoomMode == tZoomMode::NONE)
		return;

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		if (m_zoomMode == tZoomMode::HORZ)
			bResult = m_horzCoord.Zoom(bDirection);
		else if (m_zoomMode == tZoomMode::VERT)
			bResult = m_vertCoord.Zoom(bDirection);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}
