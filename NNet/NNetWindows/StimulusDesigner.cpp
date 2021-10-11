// StimulusDesigner.cpp
//
// NNetWindows

#include <math.h>       // pow
#include "stdafx.h"
#include "NNetModelReaderInterface.h"
#include "StimulusDesigner.h"

void StimulusDesigner::Start
(
	HWND                     const   hwndParent,
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

void StimulusDesigner::doPaint() const
{
	D2D1::ColorF const color        { D2D1::ColorF::Black };
	fPixel       const fPixWidth    { 1.0_fPixel };
	fPixel       const fPixYoff     { 1.0_fPixel };
	fMicroSecs   const usInWindow   { m_fMicroSecsPerPixel * m_fPixWinWidth.GetValue() };
	fMicroSecs   const usResolution { m_pNMRI->GetTimeResolution() };
	float        const fPointsInWin { usInWindow / usResolution };
	fMicroSecs   const usIncrement  { (fPointsInWin > m_fPixWinWidth.GetValue()) ? m_fMicroSecsPerPixel : usResolution };
	fMicroSecs   const usEnd        { m_pNMRI->GetSimulationTime() };
	fMicroSecs   const timeStart    { usEnd - usInWindow };
	fPixelPoint        prevPoint    { m_fPixWinWidth, fPixYoff - fHertz2fPixel(stimulusFunc(usEnd)) };

	for (fMicroSecs time = usEnd - usIncrement; time >= timeStart; time -= usIncrement)
	{
		fPixelPoint const actPoint { fMicroSecs2fPixel(time), fPixYoff - fHertz2fPixel(stimulusFunc(usEnd)) };
		m_graphics.DrawLine(prevPoint, actPoint, fPixWidth, color);
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
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize(width, height);
	m_fPixWinWidth = Convert2fPixel(PIXEL(width));
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

fHertz const StimulusDesigner::stimulusFunc(fMicroSecs const time) const
{
	float const x { time.GetValue() };
	return fHertz(m_A * x * pow(m_B, (1.0f - x)));
}

void StimulusDesigner::setMaximum(fMicroSecs const uSecs, fHertz const hertz)
{
	float const x { uSecs.GetValue() };
	float const y { hertz.GetValue() };
	float const r { 1.0f/x };
	m_A = y / exp(r - 1.0f);
	m_B = exp(r);
}

fPixel const StimulusDesigner::fHertz2fPixel(fHertz const freq) const
{
	fPixel const fPixYvalue { freq / m_fHertzPerPixel };
	assert(fPixYvalue.GetValue() < 10000.0f);
	return fPixYvalue;
}

fMicroSecs const StimulusDesigner::fPixel2fMicroSecs(fPixel const fPixX) const
{
	fPixel     const fTicks   { m_fPixWinWidth - fPixX };
	fMicroSecs const usResult { m_fMicroSecsPerPixel * fTicks.GetValue() };
	return usResult;
}

fPixel const StimulusDesigner::fMicroSecs2fPixel(fMicroSecs const usParam) const
{
	float      const fTicks { usParam / m_fMicroSecsPerPixel };
	fPixel     const fPixX  { m_fPixWinWidth - fPixel(fTicks) };
	return fPixX;
}

