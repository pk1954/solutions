// NNetWindow.cpp
//
// NNetWindows

module;

#include <sstream> 
#include "Resource.h"
#include "Windows.h"

module NNetWin32:NNetWindow;

import Win32_Util_Resource;
import Types;
import Direct2D;
import Win32_Sound;
import NNetModel;
import :NNetController;

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
	m_pController        = & controller;
	m_pMonitorWindow     = pMonitorWindow;
	m_fPixRadiusLimit    = fPixLimit;
	m_pBrushSensorCables = m_upGraphics->CreateBrush(D2D1::ColorF::Green);
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

void NNetWindow::drawSignalCable(MonitorData const& monData, SignalId const& signalId) const
{
	Signal       const* pSignal            { monData.GetConstSignalPtr(signalId) };
	SignalSource const* pSigSrc            { pSignal->GetSignalSource() };
	Sensor       const* pSensor            { Cast2Sensor(pSigSrc) };
	PixelPoint   const  pixPosScreenSignal { m_pMonitorWindow->GetTrackPosScreen(signalId) };
	if (pSensor && pixPosScreenSignal.IsNotNull())
	{
		static fPixel const HDIAMETER     { 20._fPixel };
		static fPixel const VDIAMETER     { 10._fPixel };
		PixelPoint    const pixPosSignal  { Screen2Client(pixPosScreenSignal) };
		fPixelPoint   const fPixPosSignal { Convert2fPixelPoint(pixPosSignal) };
		fPixelPoint   const fPixPosStart  { fPixPosSignal + fPixelPoint(HDIAMETER, 0.0_fPixel) };
		MicroMeterPnt const umPosSensor   { pSensor->GetPosition() };
		fPixelPoint   const fPixPosSensor { GetCoordC().Transform2fPixelPos(umPosSensor) };
		fPixelPoint   const fPixDelta     { fPixPosSensor - fPixPosStart };
		fPixel        const fPixOffset    { fPixDelta.GetX() * 0.3f };
		m_upGraphics->DrawBezier
		(
			fPixPosStart,
			fPixPosStart + fPixelPoint(fPixOffset, 0.0_fPixel),
			fPixPosSensor - fPixDelta * 0.3f,
			fPixPosSensor, 
			m_pBrushSensorCables, 
			2._fPixel
		);
		m_upGraphics->FillEllipse
		(
			fPixelEllipse(fPixPosSignal, HDIAMETER, VDIAMETER), 
			m_pBrushSensorCables
		);
	}
}

void NNetWindow::DrawSensors() const
{
	MonitorData const & monData { m_pNMRI->GetMonitorDataC() };
	m_pNMRI->GetMonitorDataC().Apply2AllSignalIdsC
	(
		[this, &monData](SignalId const& signalId) 
		{ 
			Signal const* pSignal { monData.GetConstSignalPtr(signalId) };
			pSignal->Draw(m_context, false);
			if (m_pMonitorWindow)
				drawSignalCable(monData, signalId);
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
