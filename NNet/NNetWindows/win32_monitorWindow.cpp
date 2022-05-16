// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NNetColors.h"
#include "MonitorControl.h"
#include "win32_monitorWindow.h"

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

	m_horzCoord.SetPixelSize(100.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 4000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(GetWindowHandle(), false, m_horzCoord);
	m_upHorzScale->SetOrthoOffset(Convert2fPixel(H_SCALE_HEIGHT));
	m_upHorzScale->SetOrientation(false);
	m_upHorzScale->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
	m_upHorzScale->Show(true);

	m_upMonitorControl->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
}

void MonitorWindow::Stop()
{
	DestroyWindow();
}

void MonitorWindow::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_upMonitorControl->SetModelInterface(pNMWI);
}

wstring MonitorWindow::GetCaption() const 
{ 
	return (m_upMonitorControl && m_upMonitorControl->SignalTooHigh())
           ? L"Signal too high. Use auto scale."
		   : L"Monitor";
}

void MonitorWindow::OnPaint()
{
	m_upMonitorControl->Invalidate(false);
	SetCaption();
}

bool MonitorWindow::OnSize(PIXEL const width, PIXEL const height)
{
	PIXEL const monHeight { height - H_SCALE_HEIGHT };
	PIXEL const monWidth  { width  - RIGHT_BORDER  };
	m_upMonitorControl->Move(0_PIXEL,  0_PIXEL,   width, monHeight,      true);
	m_upHorzScale     ->Move(0_PIXEL,  monHeight, width, H_SCALE_HEIGHT, true);
	m_horzCoord.SetOffset(Convert2fPixel(width - RIGHT_BORDER));
	return true;
}

void MonitorWindow::OnScaleCommand(WPARAM const wParam, BaseScale * const pScale)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case SC_LBUTTONDBLCLK:
	{
		if ( m_upMonitorControl->SignalTooHigh() )
			m_vertCoord.Zoom(m_upMonitorControl->ScaleFactor());
	}
	break;

	default:
		break;
	}
}
