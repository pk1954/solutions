// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "util.h"
#include "Resource.h"
#include "MoreTypes.h"
//#include "Segment.h"
#include "win32_scale.h"
#include "Pipe.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetColors.h"
#include "win32_sound.h"
#include "win32_util_resource.h"
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

MicroMeterRect NNetWindow::GetViewRect() const 
{ 
	return GetCoordC().Transform2MicroMeterRect(GetClPixelRect()); 
};

void NNetWindow::DrawArrowsInRect
(
	PixelRect  const & rect, 
	MicroMeter const   umSize
) const
{
	m_pNMRI->Apply2AllInRect<Pipe>
	(
		GetCoordC().Transform2MicroMeterRect(rect),	
		[this, umSize](Pipe const & s) { s.DrawArrows(m_context, umSize); } 
	);
}

void NNetWindow::DrawSensors() const
{
	try
	{
		m_pNMRI->GetConstMonitorData().Apply2AllSignalsC([this](Signal const & sig) { sig.Draw(m_context, false); });
	}
	catch (MonitorDataException const & e)
	{
		SendCommand2Application(IDM_STOP, 0);
		MonitorData::HandleException(e);
	}
}

void NNetWindow::DrawHighlightedSensor() const
{
	Signal const * const pSignal { m_pNMRI->GetConstMonitorData().GetHighlightedSignal() };
	if (pSignal)
		pSignal->Draw(m_context, true);
}

void NNetWindow::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		BeginPaint(&ps);
		if (m_context.StartFrame())
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
