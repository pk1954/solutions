// SignalDesigner.cpp
//
// NNetWindows

module;

#include <memory>
#include <cassert>
#include <string>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:SignalDesigner;

import Types;
import PixFpDimension;
import Win32_Util;
import Win32_Util_Resource;
import Win32_Controls;
import ArrowButton;
import Win32_PixelTypes;
import Direct2D;
import Scale;
import NNetCommands;
import NNetModel;
import :ComputeThread;

using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::bit_cast;
using D2D1::ColorF;

void SignalDesigner::Initialize
(
	HWND          const   hwndParent,
	ComputeThread const & computeThread,
	Observable          & runObservable,
	Observable          & dynamicModelObservable
)
{
	HWND hwndSigDes = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		L"ClassSigDesWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE,
		nullptr,
		nullptr
	);

	runObservable.RegisterObserver(*this);

	m_pComputeThread = & computeThread;

	// coords

	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 10000._MicroSecs); 
	m_horzCoord.SetPixelSize(5000.0_MicroSecs);
	m_horzCoord.SetZoomFactor(1.3f);

	m_vertCoordFreq.SetPixelSizeLimits(0.02_fHertz, 1._fHertz); 
	m_vertCoordFreq.SetPixelSize(0.5_fHertz);
	m_vertCoordFreq.SetZoomFactor(1.3f);

	m_vertCoordVolt.SetPixelSizeLimits(0.01_mV, 10.0_mV); 
	m_vertCoordVolt.SetPixelSize(0.2_mV); 
	m_vertCoordVolt.SetZoomFactor(1.3f);

	// controls

	m_upSignalControl[0] = makeSignalControl(runObservable, dynamicModelObservable);
	m_upSignalControl[1] = makeSignalControl(runObservable, dynamicModelObservable);

	m_upSignalControl[0]->SetVertCoordFreq(&m_vertCoordFreq);
	m_upSignalControl[1]->SetVertCoordVolt(&m_vertCoordVolt);
	m_upSignalControl[1]->SetVertCoordFreq(nullptr);

	m_upSignalPreview = make_unique<SignalPreview>(*this, m_horzCoord, m_vertCoordVolt);
	m_upSignalPreview->SetParentContextMenueMode(true);

	// scales

	m_upHorzScale[0] = makeHorzScale();
	m_upHorzScale[1] = makeHorzScale();
	m_upHorzScale[2] = makeHorzScale();

	m_upVertScaleFreq = make_unique<Scale<fHertz>>(hwndSigDes, true, m_vertCoordFreq);
	m_upVertScaleFreq->SetInverted(true);
	m_upVertScaleFreq->SetParentContextMenueMode(true);
	m_upVertScaleFreq->SetTicksDir(BaseScale::TICKS_LEFT);
	m_upVertScaleFreq->SetOrthoOffset(Convert2fPixel(V_SCALE_WIDTH));
	m_upVertScaleFreq->SetScaleColor(COLOR_FREQ);
	m_upVertScaleFreq->Show(false);

	for (int i = 0; i <= 1; ++i)
	{
		m_upVertScaleVolt[i] = make_unique<Scale<mV>>(hwndSigDes, true, m_vertCoordVolt);
		m_upVertScaleVolt[i]->SetInverted(true);
		m_upVertScaleVolt[i]->SetParentContextMenueMode(true);
		m_upVertScaleVolt[i]->SetScaleColor(COLOR_VOLT[i]);
		m_upVertScaleVolt[i]->Show(false);
	}

	// buttons

	m_upStimulusButton = make_unique<StimulusButton>(GetWindowHandle());

	for (int i = 0; i <= 1; ++i)
	{
		m_upArrowButton[i] = make_unique<ArrowButton>(GetWindowHandle(), IDM_SIGNAL_DESIGNER_TOGGLE);
		m_upArrowButton[i]->SetBackgroundColor(BaseScale::COL_NORMAL);
		m_upArrowButton[i]->SetWindowWidth (V_SCALE_WIDTH,  false);
		m_upArrowButton[i]->SetWindowHeight(H_SCALE_HEIGHT, false);
	}
	m_hwndPreviewButton = CreateButton
	(
		hwndSigDes,
		L"P",
		0, 0,
		V_SCALE_WIDTH.GetValue(),
		H_SCALE_HEIGHT.GetValue(),
		IDM_SIGNAL_DESIGNER_PREVIEW
	);
	Util::Show(m_hwndPreviewButton, false);
}

unique_ptr<Scale<fMicroSecs>> SignalDesigner::makeHorzScale()
{
	unique_ptr<Scale<fMicroSecs>> upScale { make_unique<Scale<fMicroSecs>>(GetWindowHandle(), false, m_horzCoord) };
	upScale->SetTicksDir(BaseScale::TICKS_DOWN);
	upScale->SetOrthoOffset(Convert2fPixel(H_SCALE_HEIGHT));
	upScale->SetParentContextMenueMode(true);
	upScale->Show(true);
	return move(upScale);
}

void SignalDesigner::SetModelInterface(NNetModelWriterInterface * const p)
{
	assert(p);
	m_upSignalControl[0]->SetModelInterface(p);
	m_upSignalControl[1]->SetModelInterface(p);
	m_upSignalPreview   ->SetModelInterface(p);
	m_pNMWI = p;
}

void SignalDesigner::AddSigGenMenu
(
	HMENU    const hPopupMenu,
	SigGenId const idSigGen
)
{
	if (IsMutable(idSigGen))
	{
		AppendMenu(hPopupMenu, MF_STRING, IDD_RENAME_SIGNAL_GENERATOR, L"Rename signal generator");
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL_GENERATOR, L"Delete signal generator");
	}
	else if (idSigGen == ADD_SIGGEN)
	{
		AppendMenu(hPopupMenu, MF_STRING, IDD_ADD_SIG_GEN_TO_MONITOR, L"Add to EEG monitor");
	}
}

LPARAM SignalDesigner::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AddSigGenMenu(hPopupMenu, m_pNMWI->GetSigGenIdSelected());
	return 0L; // will be forwarded to HandleContextMenuCommand
}

void SignalDesigner::RegisterAtSigGen(SigGenId const id)
{
	if (SignalGenerator * pSigGen { m_pNMWI->GetSigGen(id) })
	{
		pSigGen->Register(*this);
	}
}

unique_ptr<SignalControl> SignalDesigner::makeSignalControl
(
	Observable & runObservable,
	Observable & dynamicModelObservable
)
{
	auto upSignalControl = make_unique<SignalControl>
	(
		GetWindowHandle(),
		*m_pComputeThread,
		runObservable,
		dynamicModelObservable,
		&m_horzCoord 
	);
	upSignalControl->SetColor(SignalControl::tColor::FREQ, COLOR_FREQ);
	upSignalControl->SetColor(SignalControl::tColor::VOLT, COLOR_VOLT[0]);
	upSignalControl->SetParentContextMenueMode(true);
	return move(upSignalControl);
}

wstring SignalDesigner::GetCaption() const
{
	if (m_pNMWI)
	{
		if (SignalGenerator const * pSigGen { m_pNMWI->GetSigGenSelected() })
			return pSigGen->GetName();
		else 
			return L"##### no SigGen found";
	}
    return L"##### not ready";
}

void SignalDesigner::Trigger()
{
	m_upSignalControl[0]->Notify(false);
	m_upSignalControl[1]->Notify(false);
	m_upVertScaleVolt[0]->Notify(false);
	m_upVertScaleVolt[1]->Notify(false);
	if (m_bPreview)
		m_upSignalPreview->Notify(false);
	SetCaption();
	m_upStimulusButton->Enable(m_pComputeThread->IsRunning());
}

bool SignalDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDM_SIGNAL_DESIGNER_TOGGLE:
		m_bIntegrated = !m_bIntegrated;
		adjustWindowHeight();
		return true;

	case IDM_SIGNAL_DESIGNER_PREVIEW:
		m_bPreview = !m_bPreview;
		adjustWindowHeight();
		return true;

	case IDD_DELETE_SIGNAL_GENERATOR:
		SetCaption();
		return true;

	case IDD_RENAME_SIGNAL_GENERATOR:
		RenameSigGenCmd::Dialog(GetWindowHandle());
		return true;

	case IDD_ADD_SIG_GEN_TO_MONITOR:
		AddSigGen2MonitorCmd::Push(TrackNr(0));
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

void SignalDesigner::OnScaleCommand(WPARAM const wParam, LPARAM const lParam)
{
	BaseScale * const pScale { bit_cast<BaseScale *>(lParam) };
	switch (auto const wId = LOWORD(wParam))
	{
	case SC_LBUTTONDBLCLK:
		scale(pScale);
		break;

	default:
		break;
	}
}

bool SignalDesigner::OnSize(PIXEL const pixClientWidth, PIXEL const pixClientHeight)
{
	adjustLayout(pixClientWidth, pixClientHeight);
	Trigger();  // cause repaint
	return true;
}

void SignalDesigner::scale(BaseScale* const pScale)
{
	float fFactor { 1.0 };
	if (pScale == m_upHorzScale[0].get())
	{
		fFactor = m_upSignalControl[0]->ScaleFactorTimeCoord();
	}
	else if (pScale == m_upHorzScale[1].get())
	{
		fFactor = m_upSignalControl[1]->ScaleFactorTimeCoord();
	}
	else if (pScale == m_upVertScaleFreq.get())
	{
		fFactor = m_upSignalControl[0]->ScaleFactorFreqCoord();
	}
	else if (pScale == m_upVertScaleVolt[0].get())
	{
		if (m_bIntegrated)
			fFactor = m_upSignalControl[0]->ScaleFactorVoltCoord();
		else
			fFactor = m_upSignalControl[1]->ScaleFactorVoltCoord();
	}
	else if (pScale == m_upVertScaleVolt[1].get())
	{
		fFactor = m_upSignalControl[1]->ScaleFactorVoltCoord();
	}
	pScale->ZoomCoordFactor(fFactor, 0.0_fPixel);
}

void SignalDesigner::adjustWindowHeight()
{
	PIXEL const pixClientWidth  { GetClientWindowWidth() };
	PIXEL const pixClientHeight { GetClientWindowHeight() };
	int   const iNrOfTilesOld   { m_iNrOfTiles };

	m_iNrOfTiles = 2;
	if (m_bIntegrated)
		--m_iNrOfTiles;
	if (m_bPreview)
		++m_iNrOfTiles;

	PIXEL const pixClientHeightNew { pixClientHeight * m_iNrOfTiles / iNrOfTilesOld };
	PIXEL const pixWinHeight       { GetWindowHeight() };
	PIXEL const pixWinHeightNew    { pixWinHeight - pixClientHeight + pixClientHeightNew };
	SetWindowHeight(pixWinHeightNew, false);
}

void SignalDesigner::adjustLayout
(
	PIXEL const pixClientWidth, 
	PIXEL const pixClientHeight
)
{
	PIXEL const pixTileHeight    { pixClientHeight / m_iNrOfTiles };
	PIXEL const pixControlHeight { pixTileHeight  - H_SCALE_HEIGHT };
	PIXEL       pixControlWidth  { pixClientWidth - V_SCALE_WIDTH  };

	SetWindowText(m_hwndPreviewButton, m_bPreview ? L"- P" : L"+ P");
	for (int i = 0; i <= 1; ++i)
		m_upArrowButton[i]->SetDirection(!m_bIntegrated);

	if (m_bIntegrated)
	{
		pixControlWidth -= V_SCALE_WIDTH;

		m_upSignalControl[0]->SetVertCoordVolt(&m_vertCoordVolt);
		m_upVertScaleVolt[0]->SetOrthoOffset(0._fPixel);
		m_upVertScaleVolt[0]->SetTicksDir(BaseScale::TICKS_RIGHT);

		m_upVertScaleVolt[0]->Move(pixClientWidth - V_SCALE_WIDTH, 0_PIXEL,          V_SCALE_WIDTH,   pixControlHeight, true);
		m_upHorzScale    [0]->Move(V_SCALE_WIDTH,                  pixControlHeight, pixControlWidth, H_SCALE_HEIGHT,   true);
	}
	else
	{
		m_upSignalControl[0]->SetVertCoordVolt(nullptr);
		m_upVertScaleVolt[0]->SetOrthoOffset(Convert2fPixel(V_SCALE_WIDTH));
		m_upVertScaleVolt[0]->SetTicksDir(BaseScale::TICKS_LEFT);
		m_upVertScaleVolt[0]->Show(true);

		m_upHorzScale    [0]->Move(V_SCALE_WIDTH, pixControlHeight,                 pixControlWidth, H_SCALE_HEIGHT,   true);
		m_upSignalControl[1]->Move(V_SCALE_WIDTH, pixTileHeight,                    pixControlWidth, pixControlHeight, true);
		m_upVertScaleVolt[0]->Move(0_PIXEL,       pixTileHeight,                    V_SCALE_WIDTH,   pixControlHeight, true);
		m_upHorzScale    [1]->Move(V_SCALE_WIDTH, pixTileHeight + pixControlHeight, pixControlWidth, H_SCALE_HEIGHT,   true);
	}

	m_upSignalControl[0]->Move(V_SCALE_WIDTH, 0_PIXEL, pixControlWidth, pixControlHeight, true);
	m_upVertScaleFreq   ->Move(0_PIXEL, 0_PIXEL, V_SCALE_WIDTH, pixControlHeight, true);
	m_upVertScaleFreq   ->Show(true);

	m_upSignalControl[1]->Show(!m_bIntegrated);
	m_upHorzScale    [1]->Show(!m_bIntegrated);
	
	if (m_bPreview)
	{
		m_upVertScaleVolt[1]->SetOrthoOffset(Convert2fPixel(V_SCALE_WIDTH));
		m_upVertScaleVolt[1]->SetTicksDir(BaseScale::TICKS_LEFT);

		m_upSignalPreview   ->Move(V_SCALE_WIDTH, pixClientHeight - pixTileHeight,  pixClientWidth, pixControlHeight, true);
		m_upVertScaleVolt[1]->Move(0_PIXEL,       pixClientHeight - pixTileHeight,  V_SCALE_WIDTH,  pixControlHeight, true);
		m_upHorzScale    [2]->Move(V_SCALE_WIDTH, pixClientHeight - H_SCALE_HEIGHT, pixClientWidth, H_SCALE_HEIGHT,   true);

		m_upSignalPreview->Show(true);
	}

	m_upVertScaleVolt[1]->Show(m_bPreview);
	m_upHorzScale    [2]->Show(m_bPreview);

	m_upStimulusButton->CenterInParentWin();
	::SetWindowPos // TODO: use RootWindow::Move?
	(
		m_upArrowButton[0]->GetWindowHandle(),
		HWND_TOP,
		0, pixControlHeight.GetValue(), 0, 0,
		SWP_NOSIZE
	);

	if (m_bPreview)
	{
		PixelPoint pixArrowButtonPos;
		if (m_bIntegrated)
			pixArrowButtonPos = PixelPoint { pixClientWidth - V_SCALE_WIDTH, pixControlHeight };
		else
			pixArrowButtonPos = PixelPoint { 0_PIXEL, pixControlHeight + H_SCALE_HEIGHT + pixControlHeight };
		::SetWindowPos // TODO: use RootWindow::Move?
		(
			m_upArrowButton[1]->GetWindowHandle(),
			HWND_TOP,
			pixArrowButtonPos.GetXvalue(),
			pixArrowButtonPos.GetYvalue(),
			0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW
		);
	}
	else
	{
		m_upArrowButton[1]->Show(false);
	}

	PIXEL const pixPreviewButtonXpos { (m_bIntegrated && !m_bPreview) ? pixClientWidth - V_SCALE_WIDTH : 0_PIXEL };
	::SetWindowPos
	(
		m_hwndPreviewButton,
		HWND_TOP,
		pixPreviewButtonXpos.GetValue(), 
		(pixClientHeight - H_SCALE_HEIGHT).GetValue(), 
		0, 0,
		SWP_NOSIZE | SWP_SHOWWINDOW
	);

}
