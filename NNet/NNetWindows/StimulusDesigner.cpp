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

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 40000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);
	m_horzCoord.RegisterObserver(this);

	m_horzScale.InitHorzScale(&m_horzCoord, &m_graphics, L"s", 1e6f);
	m_horzScale.SetOrientation(false);
	m_horzScale.Recalc();

	m_vertCoord.SetPixelSize(0.25_fHertz);
	m_vertCoord.SetPixelSizeLimits(0.001_fHertz, 100._fHertz); 
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

fPixelPoint const StimulusDesigner::getPixPnt(fMicroSecs const time) const
{
	fHertz      const fHertzAct{ m_pSignalGenerator->GetBaseFrequency() + m_pSignalGenerator->StimulusFunc(time) };
	fPixelPoint const actPoint { m_horzCoord.Transform2fPixelPos(time), m_vertCoord.Transform2fPixelPos(-fHertzAct) };
	return actPoint;
}

void StimulusDesigner::doPaint() const
{
	D2D1::ColorF const color        { D2D1::ColorF::Black };
	fPixel       const fPixWidth    { 1.0_fPixel };
	fMicroSecs   const usResolution { m_pNMRI->TimeResolution() };
	fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs   const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
	fMicroSecs   const timeStart    { 0.0_MicroSecs };
	fMicroSecs   const timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
	fPixelPoint        prevPoint    { getPixPnt(timeStart) };

	for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
	{
		fPixelPoint const actPoint  { getPixPnt(time) };
		fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
		m_graphics.DrawLine(prevPoint, stepPoint, fPixWidth, color);
		m_graphics.DrawLine(stepPoint,  actPoint, fPixWidth, color);
		prevPoint = actPoint;
	}

	m_horzScale.Display();
	m_vertScale.Display();
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

void StimulusDesigner::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	static float const ZOOM_FACTOR { 1.3f };

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		bResult = bShiftKey 
			      ? m_horzCoord.Zoom(bDirection)
			      : m_vertCoord.Zoom(bDirection);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}
