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
	static D2D1::ColorF COLOR_FREQ { D2D1::ColorF::Green };
	static D2D1::ColorF COLOR_VOLT { D2D1::ColorF::Blue  };

	static PIXEL STD_WINDOW_HEIGHT { 450_PIXEL };

	HWND hwnd = GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassSigDesWindow", 
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE
	);

	//HWND hwnd = StartBaseWindow
	//(
	//	hwndParent,
	//	CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
	//	L"ClassSigDesWindow",
	//	WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE,
	//	nullptr,
	//	nullptr
	//);
	m_upSignalControl1 = make_unique<SignalControl>
	(
		hwnd,
		computeThread,
		m_commands,
		m_sigGen,
		&m_horzCoord 
	);
	m_upSignalControl2 = make_unique<SignalControl>
	(
		hwnd,
		computeThread,
		m_commands,
		m_sigGen,
		&m_horzCoord
	);
	runObservable  .RegisterObserver(*m_upSignalControl1.get());
	m_horzCoord    .RegisterObserver(*m_upSignalControl1.get()); 
	m_vertCoordFreq.RegisterObserver(*m_upSignalControl1.get());
	m_vertCoordVolt.RegisterObserver(*m_upSignalControl1.get());
	m_upSignalControl1->SetColor(SignalControl::tColor::FREQ, COLOR_FREQ);
	m_upSignalControl1->SetColor(SignalControl::tColor::VOLT, COLOR_VOLT);

	runObservable  .RegisterObserver(*m_upSignalControl2.get());
	m_horzCoord    .RegisterObserver(*m_upSignalControl2.get()); 
	m_vertCoordFreq.RegisterObserver(*m_upSignalControl2.get());
	m_vertCoordVolt.RegisterObserver(*m_upSignalControl2.get());
	m_upSignalControl2->SetColor(SignalControl::tColor::FREQ, COLOR_FREQ);
	m_upSignalControl2->SetColor(SignalControl::tColor::VOLT, COLOR_VOLT);

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(100._MicroSecs, 1000000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_vertCoordFreq.SetPixelSize(0.25_fHertz);
	m_vertCoordFreq.SetPixelSizeLimits(0.05_fHertz, 1._fHertz); 
	m_vertCoordFreq.SetZoomFactor(1.3f);

	mV    const mVmaxPeak         { m_sigGen.Voltage().peak };
	mV    const mVmaxPeakScaleLen { mVmaxPeak * 4.0f };
	PIXEL const pixVertScaleLen   { STD_WINDOW_HEIGHT - 0_PIXEL  - H_SCALE_HEIGHT };
	mV    const mVpixelSize       { mVmaxPeakScaleLen / static_cast<float>(pixVertScaleLen.GetValue()) };
	m_vertCoordVolt.SetPixelSize(mVpixelSize);
	m_vertCoordVolt.SetPixelSizeLimits(mVpixelSize * 0.2f, mVpixelSize * 10.f); 

	m_vertCoordVolt.SetZoomFactor(1.3f);

	m_upHorzScale1    = make_unique<Scale<fMicroSecs>>(hwnd, false, m_horzCoord);
	m_upHorzScale2    = make_unique<Scale<fMicroSecs>>(hwnd, false, m_horzCoord);
	m_upVertScaleFreq = make_unique<Scale<fHertz    >>(hwnd, true,  m_vertCoordFreq);
	m_upVertScaleVolt = make_unique<Scale<mV        >>(hwnd, true,  m_vertCoordVolt);

	m_upHorzScale1->SetOrientation(false);
	m_upHorzScale1->Show(true);

	m_upHorzScale2->SetOrientation(false);
	m_upHorzScale2->Show(true);

	m_upVertScaleFreq->SetOrientation(true);
	m_upVertScaleFreq->SetColor(COLOR_FREQ);
	m_upVertScaleFreq->Show(true);

	m_upVertScaleVolt->SetColor(COLOR_VOLT);
	m_upVertScaleVolt->Show(true);

	::MoveWindow(hwnd, 100, 100, 500, STD_WINDOW_HEIGHT.GetValue(), true);
}

void SignalDesigner::Stop()
{
	DestroyWindow();
}

void SignalDesigner::DoPaint()
{
	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Blue);
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

void SignalDesigner::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	m_design = (m_design == DESIGN::INTEGRATED) ? DESIGN::STACKED : DESIGN::INTEGRATED;
	design(GetClientWindowWidth(), GetClientWindowHeight());
	Trigger();  // cause repaint
}

bool SignalDesigner::OnSize(PIXEL const width, PIXEL const height)
{
	design(width, height);
	Trigger();  // cause repaint
	return true;
}

void SignalDesigner::design(PIXEL const width, PIXEL const height)
{
	static fPixel const fPixLeftOffset   { Convert2fPixel(V_SCALE_WIDTH ) };
	static fPixel const fPixBottomOffset { Convert2fPixel(H_SCALE_HEIGHT) };

	m_upVertScaleFreq ->SetOrthoOffset(fPixLeftOffset);
	m_upHorzScale1    ->SetOrthoOffset(fPixBottomOffset);
	m_upHorzScale2    ->SetOrthoOffset(fPixBottomOffset);
	m_upSignalControl1->SetVertCoordFreq(&m_vertCoordFreq);
	m_upSignalControl2->SetVertCoordVolt(&m_vertCoordVolt);
	m_upSignalControl2->SetVertCoordFreq(nullptr);

	PIXEL HEIGHT         { (m_design == DESIGN::INTEGRATED) ? height : height/2 };
	PIXEL CONTROL_HEIGHT { HEIGHT - H_SCALE_HEIGHT };
	PIXEL CONTROL_WIDTH;

	if (m_design == DESIGN::INTEGRATED)
	{
		CONTROL_WIDTH = width - V_SCALE_WIDTH - V_SCALE_WIDTH;

		m_upSignalControl1->SetVertCoordVolt(&m_vertCoordVolt);
		m_upVertScaleVolt ->SetOrthoOffset(0._fPixel);
		m_upVertScaleVolt ->SetOrientation(false);

		m_upVertScaleVolt ->Move(V_SCALE_WIDTH + CONTROL_WIDTH, 0_PIXEL, V_SCALE_WIDTH, CONTROL_HEIGHT, true);

		m_upSignalControl2->Show(false);
		m_upHorzScale2    ->Show(false);
	}
	else
	{
		CONTROL_WIDTH = width - V_SCALE_WIDTH;

		m_upSignalControl1->SetVertCoordVolt(nullptr);
		m_upVertScaleVolt ->SetOrthoOffset(fPixLeftOffset);
		m_upVertScaleVolt ->SetOrientation(true);

		m_upSignalControl2->Move(V_SCALE_WIDTH, HEIGHT,                  CONTROL_WIDTH, CONTROL_HEIGHT, true);
		m_upVertScaleVolt ->Move(      0_PIXEL, HEIGHT,                  V_SCALE_WIDTH, CONTROL_HEIGHT, true);
		m_upHorzScale2    ->Move(V_SCALE_WIDTH, HEIGHT + CONTROL_HEIGHT, CONTROL_WIDTH, H_SCALE_HEIGHT, true);

		m_upSignalControl2->Show(true);
		m_upHorzScale2    ->Show(true);
	}

	m_upSignalControl1->Move(V_SCALE_WIDTH,        0_PIXEL, CONTROL_WIDTH, CONTROL_HEIGHT, true);
	m_upHorzScale1    ->Move(V_SCALE_WIDTH, CONTROL_HEIGHT, CONTROL_WIDTH, H_SCALE_HEIGHT, true);
	m_upVertScaleFreq ->Move(      0_PIXEL,        0_PIXEL, V_SCALE_WIDTH, CONTROL_HEIGHT, true);
}
