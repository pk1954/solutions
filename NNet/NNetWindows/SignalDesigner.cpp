// SignalDesigner.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "PointType.h"
#include "win32_controls.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "ComputeThread.h"
#include "SignalDesigner.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"

SignalDesigner::SignalDesigner
(
	HWND               const   hwndParent,
	ComputeThread      const & computeThread,
	SignalGenerator          & sigGen,
	Observable               & runObservable,
	NNetModelCommands        & commands,
	NNetModelWriterInterface & nmwi
)
  : m_sigGen(sigGen),
	m_commands(commands),
	m_nmwi(nmwi)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassSigDesWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE,
		nullptr,
		nullptr
	);

	m_upSignalControl = make_unique<SignalControl>
	(
		hwnd,
		computeThread,
		m_commands,
		m_sigGen,
		m_horzCoord, 
		m_vertCoordFreq,
		m_vertCoordCurr
	);

	runObservable.RegisterObserver(*m_upSignalControl.get());

	static D2D1::ColorF COLOR_FREQ { D2D1::ColorF::Green };
	static D2D1::ColorF COLOR_CURR { D2D1::ColorF::Blue  };

	m_upSignalControl->SetFreqColor(COLOR_FREQ);
	m_upSignalControl->SetCurrColor(COLOR_CURR);

	m_upHorzScale     = make_unique<Scale<fMicroSecs>>(hwnd, false, m_horzCoord);
	m_upVertScaleFreq = make_unique<Scale<fHertz    >>(hwnd, true,  m_vertCoordFreq);
	m_upVertScaleCurr = make_unique<Scale<mV        >>(hwnd, true,  m_vertCoordCurr);

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(100._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_upHorzScale->SetOrientation(false);
	m_upHorzScale->Show(true);

	m_vertCoordFreq.SetPixelSize(0.25_fHertz);
	m_vertCoordFreq.SetPixelSizeLimits(0.05_fHertz, 1._fHertz); 
	m_vertCoordFreq.SetZoomFactor(1.3f);

	m_vertCoordCurr.SetPixelSize(0.25_mV);
	m_vertCoordCurr.SetPixelSizeLimits(0.05_mV, 100._mV); 
	m_vertCoordCurr.SetZoomFactor(1.3f);

	m_upVertScaleFreq->SetOrientation(true);
	m_upVertScaleFreq->SetColor(COLOR_FREQ);
	m_upVertScaleFreq->Show(true);

	m_upVertScaleCurr->SetOrientation(false);
	m_upVertScaleCurr->SetColor(COLOR_CURR);
	m_upVertScaleCurr->Show(true);


//	m_hwndTriggerButton   = CreateButton(hwnd, L"Trigger",      0, 0, 50, 20, IDM_TRIGGER_STIMULUS);
	m_hwndApply2AllButton = CreateButton(hwnd, L"Apply to all", 0, 0, 80, 20, IDD_APPLY2ALL);

	::MoveWindow(hwnd, 100, 100, 500, 450, true);
}

void SignalDesigner::Stop()
{
	DestroyWindow();
}

bool SignalDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDD_APPLY2ALL:
		m_nmwi.Apply2All<InputConnector>
		(
			[this](InputConnector & c)
			{ 
				m_commands.SetStimulusParams(c.GetSignalGenerator(), m_sigGen); 
			}
		);
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

void SignalDesigner::OnClose()
{
	Stop();
}

bool SignalDesigner::OnSize(PIXEL const width, PIXEL const height)
{
	static fPixel const fPixLeftOffset   { Convert2fPixel(LEFT_OFFSET  ) };
	static fPixel const fPixBottomOffset { Convert2fPixel(BOTTOM_OFFSET) };

	m_upHorzScale->SetOrthoOffset(fPixBottomOffset);
	m_upVertScaleFreq->SetOrthoOffset(fPixLeftOffset);

	m_upSignalControl->Move
	(
		LEFT_OFFSET,
		TOP_OFFSET,
		width  - LEFT_OFFSET - RIGHT_OFFSET,
		height - TOP_OFFSET  - BOTTOM_OFFSET,
		true
	);
	
	m_upHorzScale->Move
	(
		LEFT_OFFSET,
		height - BOTTOM_OFFSET,
		width  - LEFT_OFFSET - RIGHT_OFFSET,
		BOTTOM_OFFSET,
		true
	);

	m_upVertScaleFreq->Move
	(
		0_PIXEL,
		TOP_OFFSET,
		LEFT_OFFSET,
		height - TOP_OFFSET  - BOTTOM_OFFSET,
		true
	);

	m_upVertScaleCurr->Move
	(
		width - RIGHT_OFFSET,
		TOP_OFFSET,
		RIGHT_OFFSET,
		height - TOP_OFFSET  - BOTTOM_OFFSET,
		true
	);

	//	::SetWindowPos(m_hwndTriggerButton,   HWND_TOP, width.GetValue() - 180, height.GetValue() - 25, 0, 0, SWP_NOSIZE);
	::SetWindowPos(m_hwndApply2AllButton, HWND_TOP, width.GetValue() - 150, height.GetValue() - 25, 0, 0, SWP_NOSIZE);

	Trigger();  // cause repaint
	return true;
}
