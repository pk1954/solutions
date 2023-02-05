// Win32_NNetWindow.cpp
//
// NNetWindows

module;

#include <sstream> 
#include "Resource.h"
#include "Windows.h"

module NNetWindow;

import Win32_Util_Resource;
import Types;
import Direct2D;
import Win32_Sound;
import NNetController;
import NNetModel;

using std::function;

void NNetWindow::Start
(
	HWND            const hwndParent, 
	DWORD           const dwStyle,
	bool            const bShowRefreshRateDialog,
	fPixel          const fPixLimit,
	NNetController      & controller,
	MonitorWindow const * pMonitorWindow
)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassNNetWindow", 
		dwStyle
	);
	m_context.Start(m_upGraphics.get());
	m_pController     = & controller;
	m_pMonitorWindow  = pMonitorWindow;
	m_fPixRadiusLimit = fPixLimit;
	ShowRefreshRateDlg(bShowRefreshRateDialog);
}

NNetWindow::~NNetWindow()
{
	m_pNMRI       = nullptr;
	m_pController = nullptr;
}

void NNetWindow::SetModelInterface(NNetModelReaderInterface * const pNMRI)
{
	m_pNMRI = pNMRI;
}

MicroMeterRect NNetWindow::GetViewRect() const 
{ 
	PixelRect      pixRect = GetClPixelRect();
	MicroMeterRect umRect  = GetCoordC().Transform2logUnitRect(pixRect);
	return GetCoordC().Transform2logUnitRect(GetClPixelRect());
};

void NNetWindow::DrawArrowsInRect
(
	PixelRect  const & rect, 
	MicroMeter const   umSize
) const
{
	m_pNMRI->Apply2AllInRectC<Pipe>
	(
		GetCoordC().Transform2logUnitRect(rect),	
		[this, umSize](Pipe const & s) { s.DrawArrows(m_context, umSize); } 
	);
}

void NNetWindow::DrawSensors() const
{
	MonitorData const & monData { m_pNMRI->GetMonitorDataC() };
//	monData.Apply2AllSignalsC([this](Signal const& sig) { sig.Draw(m_context, false); });
	monData.Apply2AllSignalIdsC
	(
		[this, &monData](SignalId const& signalId) 
		{ 
			Signal const * pSignal { monData.GetConstSignalPtr(signalId) };
			pSignal->Draw(m_context, false);
			if (m_pMonitorWindow)
			{
				SignalSource const * pSigSrc { pSignal->GetSignalSource() };
				Sensor       const * pSensor { Cast2Sensor(pSigSrc) };
				if (pSensor)
				{
					TrackNr       const trackNr      { signalId.GetTrackNr() };
					fPixel        const fPixY        { m_pMonitorWindow->GetTrackPos(trackNr) };
					PixelPoint    const pixPos       { m_pMonitorWindow->GetWindowWidth(), Convert2PIXEL(fPixY) };
					PixelPoint    const pixPosScreen { m_pMonitorWindow->Client2Screen(pixPos) };
					PixelPoint    const pixPosMain   { Screen2Client(pixPosScreen) };
					fPixelPoint   const fPixPosMain  { Convert2fPixelPoint(pixPosMain) };
					MicroMeterPnt const umPosSensor  { pSensor->GetPosition() };
					MicroMeterPnt const umPosMain    { GetCoordC().Transform2logUnitPntPos(fPixPosMain) };
					m_context.DrawLine
					(
						umPosMain,
						umPosSensor,
						1.0_MicroMeter,
						D2D1::ColorF::Green,
						2._fPixel
					);
				}
			}
		}
	);
}

void NNetWindow::DrawHighlightedSensor(MacroSensor const * const pSensor) const
{
	if (pSensor)
		pSensor->Draw(m_context, true);
}

void NNetWindow::DrawSensorDataPoints(MacroSensor const * const pSensor) const
{
	if (pSensor)
		pSensor->DrawDataPoints(m_context);
}

bool NNetWindow::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	Notify(false);
	return true;    // job done
}

bool NNetWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	MicroMeterPnt const umPoint { GetCoordC().Transform2logUnitPntPos(pixPoint) };
	if (m_pController->HandleCommand(LOWORD(wParam), lParam, umPoint))
		return true;

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
