// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetColors.h"
#include "win32_sound.h"
#include "win32_NNetController.h"
#include "win32_NNetWindow.h"

using std::function;

void NNetWindow::InitClass(ActionTimer * const pActionTimer)
{
	ModelWindow::InitClass(pActionTimer);
}

NNetWindow::NNetWindow() :
	ModelWindow()
{ }

void NNetWindow::Start
(
	HWND                     const   hwndApp, 
	DWORD                    const   dwStyle,
	bool                     const   bShowRefreshRateDialog,
	fPixel                   const   fPixLimit,
	NNetModelReaderInterface const & modelReaderInterface,
	NNetController                 & controller
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		nullptr
	);
	m_graphics.Initialize(hwnd);
	m_context.Start(& m_graphics);
	m_pNMRI           = & modelReaderInterface;
	m_pController     = & controller;
	m_fPixRadiusLimit = fPixLimit;
	
	ShowRefreshRateDlg(bShowRefreshRateDialog);
}

void NNetWindow::Stop()
{
	m_context.Stop();
	DestroyWindow();
}

NNetWindow::~NNetWindow()
{
	m_pNMRI       = nullptr;
	m_pController = nullptr;
}

MicroMeterRect const NNetWindow::GetViewRect() const 
{ 
	return GetCoordC().Transform2MicroMeterRect(GetClPixelRect()); 
};

void NNetWindow::DrawInteriorInRect
(
	PixelRect const & rect, 
	NobCrit const & crit 
) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPNobs().Apply2AllInRect<Nob>
	(
		umRect,
		[&](Nob const & s) 
		{ 
			if (crit(s)) 
				s.DrawInterior(m_context, tHighlight::normal); 
		} 
	);
}

void NNetWindow::DrawExteriorInRect(PixelRect const & rect) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPNobs().Apply2AllInRect<Nob>
	(
		umRect,
		[&](Nob const & s) 
		{ 
			s.DrawExterior(m_context, tHighlight::normal); 
		} 
	);
}

void NNetWindow::DrawArrowsInRect(PixelRect const & rect, MicroMeter const umSize) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPNobs().Apply2AllInRect<Pipe>
	(
		umRect,	
		[&](Pipe const & s) { s.DrawArrows(m_context, umSize); } 
	);
}

void NNetWindow::DrawNeuronTextInRect(PixelRect const & rect) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	if (PixelSize() <= 2.5_MicroMeter)
	{
		m_pNMRI->GetUPNobs().Apply2AllInRect<Neuron>
		(
			umRect,
			[&](Neuron const & n) { n.DrawNeuronText(m_context); } 
		);
	}
}

void NNetWindow::DrawSensors() const
{
	MonitorData    const & mon     { m_pNMRI->GetMonitorData() };
	Signal const * const   pSignal { mon.GetHighlightedSignal() };
	mon.Apply2AllSignals([&](Signal const & sig) { sig.Draw(m_context, false); });
}

void NNetWindow::DrawHighlightedSensor() const
{
	MonitorData    const & mon     { m_pNMRI->GetMonitorData() };
	Signal const * const   pSignal { mon.GetHighlightedSignal() };
	if (pSignal)
		pSignal->Draw(m_context, true);
}

void NNetWindow::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint(&ps);
		if (m_context.StartFrame(hDC))
		{
			doPaint();
			m_context.EndFrame();
		}
		EndPaint(&ps);
	}
}

bool NNetWindow::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_context.Resize(width, height);
	Notify(false);
	return true;    // job done
}

bool NNetWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	MicroMeterPnt const umPoint { GetCoordC().Transform2MicroMeterPntPos(pixPoint) };
	if (m_pController->HandleCommand(LOWORD(wParam), lParam, umPoint))
		return true;

	return ModelWindow::OnCommand(wParam, lParam, pixPoint);
}
