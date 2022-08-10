// win32_monitorWindow.cpp
//
// NNetWindows

#include "Resource.h"
#include "NNetColors.h"
#include "win32_util_resource.h"
#include "win32_monitorWindow.h"

import PixelTypes;
import SoundInterface;
import MonitorControl;

using std::find;
using std::make_unique;

MonitorWindow:: MonitorWindow() = default;
MonitorWindow::~MonitorWindow() = default;

void MonitorWindow::Start
(
	HWND        const   hwndParent,
	Sound             & sound,
	NNetModelCommands & modelCmds
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassMonitorWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE,
		nullptr,
		nullptr
	);

	m_upMonitorControl = make_unique<MonitorControl>
	(
		hwnd, 
		sound, 
		modelCmds, 
		m_horzCoord, 
		m_vertCoord
	);

	m_horzCoord.SetPixelSize(DEFAULT_PIXEL_SIZE); 
	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 1e6_MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(GetWindowHandle(), false, m_horzCoord);
	m_upHorzScale->SetOrthoOffset(Convert2fPixel(H_SCALE_HEIGHT));
	m_upHorzScale->SetOrientation(false);
	m_upHorzScale->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
	m_upHorzScale->SetAllowUnlock(true);
	m_upHorzScale->Show(true);

	m_upMonitorControl->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
}

void MonitorWindow::ResetHorzCoord()
{
	m_horzCoord.SetPixelSize(DEFAULT_PIXEL_SIZE); 
	m_horzCoord.SetOffset(Convert2fPixel(RIGHT_BORDER - GetClientWindowWidth()));
}

void MonitorWindow::Stop()
{
	DestroyWindow();
}

void MonitorWindow::SetModelInterface(NNetModelWriterInterface * const pNMWI) const
{
	m_upMonitorControl->SetModelInterface(pNMWI);
}

void MonitorWindow::StimulusTriggered() const
{
	m_upMonitorControl->StimulusTriggered();
}

wstring MonitorWindow::GetCaption() const 
{ 
	return (m_upMonitorControl && m_upMonitorControl->SignalTooHigh())
           ? L"Signal too high. Use auto scale."
		   : L"Monitor";
}

bool MonitorWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId = LOWORD(wParam))
	{
	case IDM_SCALE_LOCK2ZERO:
		ResetHorzCoord();
		break;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

void MonitorWindow::OnPaint()
{
	m_upMonitorControl->Invalidate(false);
	SetCaption();
}

bool MonitorWindow::OnSize(PIXEL const width, PIXEL const height)
{
	PIXEL const monHeight { height - H_SCALE_HEIGHT };
	PIXEL const monWidth  { width  - RIGHT_BORDER };
	m_upMonitorControl->Move(0_PIXEL,  0_PIXEL,   width, monHeight,      true);
	m_upHorzScale     ->Move(0_PIXEL,  monHeight, width, H_SCALE_HEIGHT, true);
	if (m_upHorzScale->IsScaleLocked())
		m_horzCoord.SetOffset(Convert2fPixel(-monWidth));
	return true;
}
