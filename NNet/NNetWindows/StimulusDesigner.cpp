// StimulusDesigner.cpp
//
// NNetWindows

#include <math.h>       // pow
#include "stdafx.h"
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

fPixelPoint const StimulusDesigner::getPixPnt(fMicroSecs const time) const
{
	fHertz      const fHertzAct{ m_pSignalGenerator->StimulusFunc(time) };
	fPixelPoint const actPoint { m_fPixXoff + fMicroSecs2fPixel(time), m_fPixYoff - fHertz2fPixel(fHertzAct) };
	return actPoint;
}

void StimulusDesigner::doPaint() const
{
	D2D1::ColorF const color        { D2D1::ColorF::Black };
	fPixel       const fPixWidth    { 1.0_fPixel };
	fMicroSecs   const usResolution { m_pNMRI->GetTimeResolution() };
	fMicroSecs   const usIncrement  { (m_fMicroSecsPerPixel > usResolution) ? m_fMicroSecsPerPixel : usResolution };
	fMicroSecs   const timeStart    { 0.0_MicroSecs };
	fMicroSecs   const timeEnd      { fPixel2fMicroSecs(m_fPixGraphWidth) };
	fPixelPoint        prevPoint    { getPixPnt(timeStart) };

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint { getPixPnt(time) };
//		m_graphics.DrawLine(prevPoint, actPoint, fPixWidth, color);
		m_graphics.DrawLine(prevPoint, fPixelPoint(actPoint.GetX(), prevPoint.GetY()), fPixWidth, color);
		m_graphics.DrawLine(fPixelPoint(actPoint.GetX(), prevPoint.GetY()), actPoint, fPixWidth, color);
		prevPoint = actPoint;
	}
}

void StimulusDesigner::OnPaint()
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

bool StimulusDesigner::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	static const float HORZ_BORDER { 0.1f };
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize(width, height);
	m_fPixWinWidth   = Convert2fPixel(PIXEL(width));
	m_fPixWinHeight  = Convert2fPixel(PIXEL(height));
	m_fPixXoff       = m_fPixWinWidth  * HORZ_BORDER;
	m_fPixYoff       = m_fPixWinHeight * (1.0f - HORZ_BORDER);
	m_fPixGraphWidth = m_fPixWinWidth  * (1.0f - 2.0f * HORZ_BORDER);
	Trigger();  // cause repaint
	return true;
}

void StimulusDesigner::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	static float const ZOOM_FACTOR { 1.3f };

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };

	for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
	{
		if (bShiftKey)
		{
			static const fMicroSecs LOWER_LIMIT {   1.0_MicroSecs };
			static const fMicroSecs UPPER_LIMIT { 400.0_MicroSecs };
			if (bDirection)
			{
				if (m_fMicroSecsPerPixel > LOWER_LIMIT)
					m_fMicroSecsPerPixel /= ZOOM_FACTOR;
				else 
					MessageBeep(MB_ICONWARNING);
			}
			else
			{
				if (m_fMicroSecsPerPixel < UPPER_LIMIT)
					m_fMicroSecsPerPixel *= ZOOM_FACTOR;
				else 
					MessageBeep(MB_ICONWARNING);
			}
		}
		else
		{
			static const fHertz LOWER_LIMIT { 0.001_fHertz };
			static const fHertz UPPER_LIMIT { 100.0_fHertz };
			if (bDirection)
			{
				if (m_fHertzPerPixel > LOWER_LIMIT)
					m_fHertzPerPixel /= ZOOM_FACTOR;
				else 
					MessageBeep(MB_ICONWARNING);
			}
			else
			{
				if (m_fHertzPerPixel < UPPER_LIMIT)
					m_fHertzPerPixel *= ZOOM_FACTOR;
				else 
					MessageBeep(MB_ICONWARNING);
			}
		}
	}

	Trigger();  // cause repaint
}

fPixel const StimulusDesigner::fHertz2fPixel(fHertz const freq) const
{
	fPixel const fPixYvalue { freq / m_fHertzPerPixel };
	assert(fPixYvalue.GetValue() < 10000.0f);
	return fPixYvalue;
}

fMicroSecs const StimulusDesigner::fPixel2fMicroSecs(fPixel const fPixX) const
{
	fMicroSecs const usResult { m_fMicroSecsPerPixel * fPixX.GetValue() };
	return usResult;
}

fPixel const StimulusDesigner::fMicroSecs2fPixel(fMicroSecs const usParam) const
{
	float  const fTicks { usParam / m_fMicroSecsPerPixel };
	fPixel const fPixX  { fPixel(fTicks) };
	return fPixX;
}

