// MonitorWindow.cpp
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <algorithm>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:MonitorWindow;

import Win32_Util_Resource;
import Types;
import Observable;
import SoundInterface;
import NNetModel;
import :ComputeThread;
import :MonitorControl;

using std::find;
using std::wstring;
using std::make_unique;

MonitorWindow:: MonitorWindow() = default;
MonitorWindow::~MonitorWindow() = default;

void MonitorWindow::Start
(
	HWND          const   hwndParent,
	ComputeThread const & computeThread,
	Sound               & sound,
	Observable          & observable
)
{
	m_pMoveSizeObservable = &observable;
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassMonitorWindow",
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SIZEBOX | WS_VISIBLE,
		nullptr,
		nullptr
	);

	m_upMonitorControl = make_unique<MonitorControl>
	(
		hwnd, 
		sound, 
		m_horzCoord, 
		m_vertCoord,
		observable
	);

	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 1e6_MicroSecs); 
	m_horzCoord.SetPixelSize(DEFAULT_PIXEL_SIZE); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(hwnd, false, m_horzCoord);
	m_upHorzScale->SetOrthoOffset(Convert2fPixel(H_SCALE_HEIGHT));
	m_upHorzScale->SetTicksDir(BaseScale::TICKS_DOWN);
	m_upHorzScale->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
	m_upHorzScale->SetAllowUnlock(true);
	m_upHorzScale->Show(true);

	m_upMonitorControl->SetRightBorder(Convert2fPixel(RIGHT_BORDER));

	m_pComputeThread   = & computeThread;
	m_upStimulusButton = make_unique<StimulusButton>(GetWindowHandle());
}

void MonitorWindow::ResetHorzCoord()
{
	m_horzCoord.SetPixelSize(DEFAULT_PIXEL_SIZE); 
	m_horzCoord.SetOffset(Convert2fPixel(RIGHT_BORDER - GetClientWindowWidth()));
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
	m_upStimulusButton->Enable(m_pComputeThread->IsRunning());
}

bool MonitorWindow::OnSize(PIXEL const pixClientWidth, PIXEL const pixClientHeight)
{
	PIXEL const monHeight { pixClientHeight - H_SCALE_HEIGHT };
	PIXEL const monWidth  { pixClientWidth  - RIGHT_BORDER };
	m_upMonitorControl->Move(0_PIXEL,  0_PIXEL,   pixClientWidth, monHeight,      true);
	m_upHorzScale     ->Move(0_PIXEL,  monHeight, pixClientWidth, H_SCALE_HEIGHT, true);
	if (m_upHorzScale->IsScaleLocked())
		m_horzCoord.SetOffset(Convert2fPixel(-monWidth));
	m_upStimulusButton->CenterInParentWin();
	m_pMoveSizeObservable->NotifyAll(false);
	return true;
}

bool MonitorWindow::OnMove(PIXEL const pixPosX, PIXEL const pixPosY)
{ 
	m_pMoveSizeObservable->NotifyAll(false);
	return BaseWindow::OnMove(pixPosX, pixPosY);
};
