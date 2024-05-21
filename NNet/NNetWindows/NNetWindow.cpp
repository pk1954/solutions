// NNetWindow.cpp
//
// NNetWindows

module;

#include <memory>
#include <cassert>
#include <sstream> 
#include "Resource.h"
#include "Windows.h"
#include <optional>

module NNetWin32:NNetWindow;

import Win32_Util_Resource;
import Types;
import Direct2D;
import Win32_Sound;
import NNetModel;
import NNetPreferences;
import :NNetCommandHandler;

using std::function;
using std::optional;
using std::unique_ptr;

static Color const EEG_SIGNAL_HIGH { 1.0f, 0.5f, 0.0f, 1.0f };

void NNetWindow::Start
(
	HWND            const hwndParent, 
	DWORD           const dwStyle,
	bool            const bShowRefreshRateDialog,
	NNetCommandHandler  & controller,
	MonitorWindow       * pMonitorWindow
)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassNNetWindow", 
		dwStyle
	);
	m_context.Start(m_upGraphics.get());
	SetDefaultBackgroundColor();
	m_pCmdHandler          = & controller;
	m_pMonitorWindow       = pMonitorWindow;
	m_pBrushSensorNormal   = m_upGraphics->CreateBrush(NNetColors::MICRO_SENSOR);
	m_pBrushSensorSelected = m_upGraphics->CreateBrush(EEG_SIGNAL_HIGH);
	ShowRefreshRateDlg(bShowRefreshRateDialog);
}

NNetWindow::~NNetWindow()
{
	m_pNMRI       = nullptr;
	m_pCmdHandler = nullptr;
}

void NNetWindow::SetDefaultBackgroundColor()
{
	m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
}

void NNetWindow::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
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

SignalId NNetWindow::FindSignalHandle(MicroMeterPnt const& umPos) const
{
	SignalId sigIdResult;
	if (m_pMonitorWindow)
	{
		fPixelPoint const fPixPos { GetCoordC().Transform2fPixelPos(umPos) };
		m_pNMRI->GetMonitorDataC().Apply2AllSignalIdsC
		(
			[this, &fPixPos, &sigIdResult](SignalId const& sigIdRun)
			{
				PixelPoint const pixPosScreenSignal { m_pMonitorWindow->GetTrackPosScreen(sigIdRun, tHorzDir::right) };
				if (pixPosScreenSignal.IsNotNull())
				{
					PixelPoint  const pixPosSignal  { Screen2Client(pixPosScreenSignal) };
					fPixelPoint const fPixPosSignal { Convert2fPixelPoint(pixPosSignal) };
					if (Distance(fPixPosSignal, fPixPos) <= HRADIUS)
						sigIdResult = sigIdRun;
				}
			}
		);
	}
	return sigIdResult;
}

void NNetWindow::DrawSensors() const
{
	m_pNMRI->GetMonitorDataC().Apply2AllSignalIdsC
	(
		[this](SignalId const& signalId) 
		{ 
			MonitorData      const& monData     { m_pNMRI->GetMonitorDataC() };
			Signal           const* pSignal     { monData.GetConstSignalPtr(signalId) };
			NNetSignal       const* pNNetSignal { static_cast<NNetSignal const*>(pSignal) };
			NNetSignalSource const* pSigSrc     { pNNetSignal->GetSignalSource() };
			if (!pSigSrc->IsGenerator()) 
			{
				pSigSrc->Draw(m_context, false);
				if (m_pMonitorWindow && m_pMonitorWindow->IsWindowVisible())
				{
					ID2D1SolidColorBrush* const pBrush
					{
						(m_pMonitorWindow->IsSignalHighlighted(signalId) && (monData.GetNrOfSignals() > 1))
						? m_pBrushSensorSelected
						: m_pBrushSensorNormal
					};
					drawSignalCable(signalId, pSigSrc->GetPosition(), *pBrush);
				}
			}
		}
	);
}

void NNetWindow::CenterAndZoomRect
(
	Uniform2D<MicroMeter> &coordTarget,
	MicroMeterRect   const umRect,
	float            const fRatioFactor 
)
{
	MicroMeter const umZoomFactor 
	{ 
		coordTarget.ComputeZoom(umRect, GetClRectSize(), fRatioFactor) 
	};
	coordTarget.SetPixelSize(umZoomFactor, false);    // do not change order!
		
	fPixelPoint const fPixOffset   
	{ 
		coordTarget.Transform2fPixelSize(umRect.GetCenter())   // SetPixelSize result is used here
		- Convert2fPixelPoint(GetClRectCenter()) 
	}; 
	coordTarget.SetPixelOffset(fPixOffset, false);  // do not change order! 

	coordTarget.NotifyAll(true);
}

void NNetWindow::drawSignalCable
(
	SignalId       const& signalId,
	MicroMeterPnt  const& umPosSensor,
	ID2D1SolidColorBrush& brush
) const
{
	if (!m_pMonitorWindow)
		return;

	PixelPoint const pixPosScreenSignal { m_pMonitorWindow->GetTrackPosScreen(signalId, tHorzDir::right) };
	if (pixPosScreenSignal.IsNull())
		return;

	static fPixel const HRADIUS       { 20._fPixel };
	static fPixel const VRADIUS       { 10._fPixel };
	PixelPoint    const pixPosSignal  { Screen2Client(pixPosScreenSignal) };
	fPixelPoint   const fPixPosSignal { Convert2fPixelPoint(pixPosSignal) };
	fPixelPoint   const fPixPosStart  { fPixPosSignal + fPixelPoint(HRADIUS, 0.0_fPixel) };
	fPixelPoint   const fPixPosSensor { GetCoordC().Transform2fPixelPos(umPosSensor) };
	fPixelPoint   const fPixDelta     { fPixPosSensor - fPixPosStart };
	fPixel        const fPixOffset    { abs(fPixDelta.GetXvalue()) * 0.3f };

	m_upGraphics->DrawBezier
	(
		fPixPosStart,
		fPixPosStart + fPixelPoint(fPixOffset, 0.0_fPixel),
		fPixPosSensor - fPixDelta * 0.3f,
		fPixPosSensor,
		&brush,
		2._fPixel
	);
	m_upGraphics->FillEllipse
	(
		fPixelEllipse(fPixPosSignal, HRADIUS, VRADIUS),
		&brush
	);
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
	if (m_pCmdHandler->HandleCommand(LOWORD(wParam), lParam, umPoint))
		return true;

	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
