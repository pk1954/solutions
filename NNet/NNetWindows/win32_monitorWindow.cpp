// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
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

	m_upMonitorControl = make_unique<MonitorControl>(hwnd, sound, modelCmds, m_horzCoord);

	m_horzCoord.SetPixelSize(100.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 4000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(GetWindowHandle(), false, m_horzCoord);
	m_upHorzScale->SetOrthoOffset(Convert2fPixel(SCALE_HEIGHT));
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

wstring const & MonitorWindow::GetTitle() const 
{ 
	static wstring const CAPTION { L"Monitor" };
	static wstring const WARNING { L"Signal too high. Use auto scale." };
	return (m_upMonitorControl && m_upMonitorControl->SignalTooHigh())
           ? WARNING
		   : CAPTION;
}

void MonitorWindow::OnPaint()
{
	m_upMonitorControl->Invalidate(false);
	SetCaption();
}

bool MonitorWindow::OnSize(PIXEL const width, PIXEL const height)
{
	PIXEL const monHeight { height - SCALE_HEIGHT };
	m_upMonitorControl->Move(0_PIXEL, 0_PIXEL,   width, monHeight,    true);
	m_upHorzScale     ->Move(0_PIXEL, monHeight, width,	SCALE_HEIGHT, true);
	m_horzCoord.SetOffset(Convert2fPixel(width-RIGHT_BORDER));
	return true;
}
