// MonitorWindow.cpp
//
// NNetSignals

module NNetSignals:MonitorWindow;

import std;
import Win32_Util_Resource;
import WinBasics;
import Types;
import Observable;
import SoundInterface;
import NNetModel;
import :SimuRunning;
import :MonitorControl;
import Resource;

using std::abs;
using std::find;
using std::wstring;
using std::make_unique;

MonitorWindow:: MonitorWindow() = default;
MonitorWindow::~MonitorWindow() = default;

void MonitorWindow::Start
(
	HWND const         hwndParent,
	SimuRunning const &simuRunning,
	Sound             &sound,
	Observable        &observable
)
{
	m_pMoveSizeObservable = &observable;
	m_pSimuRunning        = &simuRunning;
	m_pSound              = &sound;
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
		m_horzCoord, 
		m_vertCoord,
		observable
	);

	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 1e6_MicroSecs); 
	m_horzCoord.SetPixelSize(DEFAULT_PIXEL_SIZE); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale = make_unique<Scale<fMicroSecs>>(hwnd, false, sound, m_horzCoord);
	m_upHorzScale->SetOrthoOffset(Convert2fPixel(H_SCALE_HEIGHT));
	m_upHorzScale->SetTicksDir(BaseScale::TICKS_DOWN);
	m_upHorzScale->SetRightBorder(Convert2fPixel(RIGHT_BORDER));
	m_upHorzScale->SetAllowUnlock(true);
	m_upHorzScale->Show(true);

	m_upMonitorControl->SetRightBorder(Convert2fPixel(RIGHT_BORDER));

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

wstring MonitorWindow::GetCaption() const 
{ 
	return (m_upMonitorControl && m_upMonitorControl->SignalTooHigh())
           ? L"Signal too high. Use auto scale."
		   : L"Monitor";
}

bool MonitorWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId = LoWord(wParam))
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
	m_upStimulusButton->Enable(m_pSimuRunning->IsRunning());
}

bool MonitorWindow::OnSize(PIXEL const width, PIXEL const height)
{
	::ArrangeVertical(m_upMonitorControl.get(), m_upHorzScale.get());
	if (m_upHorzScale->IsScaleLocked())
		m_horzCoord.SetOffset(Convert2fPixel(RIGHT_BORDER - width));
	m_upStimulusButton->CenterInParentWin();
	m_pMoveSizeObservable->NotifyAll();
	return true;
}

bool MonitorWindow::OnMove(PIXEL const pixPosX, PIXEL const pixPosY)
{ 
	m_pMoveSizeObservable->NotifyAll();
	return BaseWindow::OnMove(pixPosX, pixPosY);
};

bool MonitorWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	int  const iDelta     { MouseWheelDelta(wParam) };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		bResult = bShiftKey 
		? m_horzCoord.ZoomDir(bDirection, 0.0_fPixel)
		: m_vertCoord.ZoomDir(bDirection, 0.0_fPixel);
	}
	if (!bResult)
		m_pSound->WarningSound();
	return true;
}
