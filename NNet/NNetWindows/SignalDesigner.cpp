// SignalDesigner.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NNetParameters.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "win32_arrowButton.h"
#include "win32_controls.h"
#include "win32_editLineBox.h"
#include "win32_util_resource.h"
#include "win32_PixelTypes.h"
#include "SignalDesigner.h"

import PointType;
import ComputeThread;

using std::bit_cast;

void SignalDesigner::Initialize
(
	HWND          const   hwndParent,
	ComputeThread const & computeThread,
	Observable          & runObservable,
	Observable          & dynamicModelObservable,
	NNetModelCommands   * pCommands
)
{
	HWND hwndSigDes = GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassSigDesWindow", 
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE
	);

	runObservable.RegisterObserver(*this);

	m_pComputeThread = & computeThread;
	m_pCommands = pCommands;

	m_horzCoord.SetPixelSize(10000.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 100000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);

	m_vertCoordFreq.SetPixelSize(0.25_fHertz);
	m_vertCoordFreq.SetPixelSizeLimits(0.02_fHertz, 1._fHertz); 
	m_vertCoordFreq.SetZoomFactor(1.3f);

	m_vertCoordVolt1.SetPixelSize(0.1_mV); 
	m_vertCoordVolt1.SetPixelSizeLimits(0.01_mV, 10.0_mV); 
	m_vertCoordVolt1.SetZoomFactor(1.3f);

	m_vertCoordVolt2.SetPixelSize(0.1_mV); 
	m_vertCoordVolt2.SetPixelSizeLimits(0.01_mV, 10.0_mV); 
	m_vertCoordVolt2.SetZoomFactor(1.3f);

	m_upHorzScale1     = make_unique<Scale<fMicroSecs>>(hwndSigDes, false, m_horzCoord);
	m_upHorzScale2     = make_unique<Scale<fMicroSecs>>(hwndSigDes, false, m_horzCoord);
	m_upHorzScale3     = make_unique<Scale<fMicroSecs>>(hwndSigDes, false, m_horzCoord);
	m_upVertScaleFreq  = make_unique<Scale<fHertz    >>(hwndSigDes, true,  m_vertCoordFreq);
	m_upVertScaleVolt1 = make_unique<Scale<mV        >>(hwndSigDes, true,  m_vertCoordVolt1);
	m_upVertScaleVolt2 = make_unique<Scale<mV        >>(hwndSigDes, true,  m_vertCoordVolt2);

	m_upHorzScale1->SetOrientation(false);
	m_upHorzScale1->Show(true);

	m_upHorzScale2->SetOrientation(false);
	m_upHorzScale2->Show(true);

	m_upHorzScale3->SetOrientation(false);
	m_upHorzScale3->Show(true);

	m_upVertScaleFreq->SetOrientation(true);
	m_upVertScaleFreq->SetScaleColor(COLOR_FREQ);
	m_upVertScaleFreq->Show(true);

	m_upVertScaleVolt1->SetScaleColor(COLOR_VOLT);
	m_upVertScaleVolt1->Show(true);

	m_upVertScaleVolt2->SetScaleColor(D2D1::ColorF::Black);
	m_upVertScaleVolt2->Show(true);

	m_upSignalControl1 = makeSignalControl(computeThread, runObservable, dynamicModelObservable);
	m_upSignalControl2 = makeSignalControl(computeThread, runObservable, dynamicModelObservable);
	m_upSignalPreview  = make_unique<SignalPreview>(*this, m_horzCoord, m_vertCoordVolt2);

	m_upHorzScale1    ->SetParentContextMenueMode(true);
	m_upHorzScale2    ->SetParentContextMenueMode(true);
	m_upHorzScale3    ->SetParentContextMenueMode(true);
	m_upVertScaleFreq ->SetParentContextMenueMode(true);
	m_upVertScaleVolt1->SetParentContextMenueMode(true);
	m_upVertScaleVolt2->SetParentContextMenueMode(true);
	m_upSignalControl1->SetParentContextMenueMode(true);
	m_upSignalControl2->SetParentContextMenueMode(true);
	m_upSignalPreview ->SetParentContextMenueMode(true);
	m_hStimulusButton =	CreateButton
	(
		GetWindowHandle(), 
		L" Stimulus ", 
		0, 0, 
		STIMULUS_BUTTON_WIDTH .GetValue(), 
		STIMULUS_BUTTON_HEIGHT.GetValue(),
		IDM_TRIGGER_STIMULUS
	);
	BringWindowToTop(m_hStimulusButton);

	m_upArrowButton = make_unique<ArrowButton>(GetWindowHandle(), IDM_SIGNAL_DESIGNER_STACKED);
	m_upArrowButton->SetBackgroundColor(BaseScale::COL_NORMAL);
	m_upArrowButton->SetWindowWidth (V_SCALE_WIDTH,  false);
	m_upArrowButton->SetWindowHeight(H_SCALE_HEIGHT, false);
}

void SignalDesigner::SetModelInterface(NNetModelWriterInterface * const p)
{
	assert(p);
	m_upSignalControl1->SetModelInterface(p);
	m_upSignalControl2->SetModelInterface(p);
	m_upSignalPreview ->SetModelInterface(p);
	m_pNMWI = p;
}

LPARAM SignalDesigner::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AppendMenu(hPopupMenu, MF_STRING, IDD_SELECT_SIG_GEN_CLIENTS,  L"Select related input lines");
	if (m_pNMWI->GetSigGenSelected() != m_pNMWI->GetSigGenStandard())
	{
		AppendMenu(hPopupMenu, MF_STRING, IDD_RENAME_SIGNAL_GENERATOR, L"Rename signal generator");
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL_GENERATOR, L"Delete signal generator");
	}
	AppendMenu(hPopupMenu, MF_STRING, IDD_ADD_SIG_GEN_TO_MONITOR,  L"Add to EEG monitor");
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
	ComputeThread const & computeThread,
	Observable          & runObservable,
	Observable          & dynamicModelObservable
)
{
	auto upSignalControl = make_unique<SignalControl>
	(
		GetWindowHandle(),
		computeThread,
		*m_pCommands,
		runObservable,
		dynamicModelObservable,
		&m_horzCoord 
	);
	upSignalControl->SetColor(SignalControl::tColor::FREQ, COLOR_FREQ);
	upSignalControl->SetColor(SignalControl::tColor::VOLT, COLOR_VOLT);
	return move(upSignalControl);
}

void SignalDesigner::Stop()
{
	DestroyWindow();
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

void SignalDesigner::DoPaint()
{
	m_upGraphics->FillBackground(BaseScale::COL_NORMAL);
	m_upSignalControl1->Notify(false);
	m_upSignalControl2->Notify(false);
	if (m_bPreview)
		m_upSignalPreview->Notify(false);
	SetCaption();
	EnableWindow(m_hStimulusButton, m_pComputeThread->IsRunning());
}

void SignalDesigner::renameSigGen()
{
	wstring wstrName { m_pNMWI->GetSigGenSelected()->GetName() };
	EditLineBox dlgBox(wstrName);
	dlgBox.Show(GetWindowHandle());
	m_pCommands->RenameSigGen(m_pNMWI->GetSigGenIdSelected(), wstrName);
	SetCaption();
}

void SignalDesigner::toggleDesign()
{
	PIXEL clientHeight    { GetClientWindowHeight() };
	PIXEL newClientHeight { (m_design == DESIGN::INTEGRATED) ? (clientHeight*2) : (clientHeight/2) };
	PIXEL newWinHeight    { GetWindowHeight() + newClientHeight - clientHeight };
	SetWindowHeight(newWinHeight, false);
	m_design = (m_design == DESIGN::INTEGRATED) ? DESIGN::STACKED : DESIGN::INTEGRATED;
	m_upArrowButton->SetDirection(m_design == DESIGN::STACKED);
	design(GetClientWindowWidth(), GetClientWindowHeight());
	m_upArrowButton->Trigger();
}

bool SignalDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDM_SIGNAL_DESIGNER_INTEGRATED:
	case IDM_SIGNAL_DESIGNER_STACKED:
		toggleDesign();
		return true;

	case IDD_DELETE_SIGNAL_GENERATOR:
		m_pCommands->DeleteSigGen();
		return true;

	case IDD_RENAME_SIGNAL_GENERATOR:
		renameSigGen();
		return true;

	case IDD_SELECT_SIG_GEN_CLIENTS:
		m_pCommands->SelectSigGenClients();
		return true;

	case IDD_ADD_SIG_GEN_TO_MONITOR:
		m_pCommands->AddSigGen2Monitor(TrackNr(0));
		return true;

	case IDM_TRIGGER_STIMULUS:
		m_pNMWI->GetSigGenSelected()->StartStimulus();
		SendCommand2Application(IDM_TRIGGER_STIMULUS, 0);
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

void SignalDesigner::OnNCLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	SigGenId             sigGenId   { m_pNMWI->GetSigGenIdSelected() };
	UPSigGenList const & sigGenList { m_pNMWI->GetSigGenList() };
	sigGenId = (sigGenId.GetValue() + 1) % sigGenList.Size();
	PostCommand2Application(IDD_SELECT_SIGNAL_GENERATOR, sigGenId.GetValue());
}

void SignalDesigner::OnScaleCommand(WPARAM const wParam, BaseScale * const pScale)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case SC_LBUTTONDBLCLK:
		{
			float fFactor { 1.0 };
			if (pScale == m_upHorzScale1.get())
			{
				fFactor = m_upSignalControl1->ScaleFactorTimeCoord();
			}
		    else if (pScale == m_upHorzScale2.get())
			{
				fFactor = m_upSignalControl2->ScaleFactorTimeCoord();
			}
			else if (pScale == m_upVertScaleFreq.get())
			{
				fFactor = m_upSignalControl1->ScaleFactorFreqCoord();
			}
			else if (pScale == m_upVertScaleVolt1.get())
			{
				if (m_design == DESIGN::INTEGRATED)
					fFactor = m_upSignalControl1->ScaleFactorVoltCoord();
				else
					fFactor = m_upSignalControl2->ScaleFactorVoltCoord();
			}
			else if (pScale == m_upVertScaleVolt2.get())
			{
				fFactor = m_upSignalControl2->ScaleFactorVoltCoord();
			}
			pScale->ZoomCoordFactor(fFactor, 0.0_fPixel);
		}
		break;

	default:
		break;
	}
}

bool SignalDesigner::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
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
	m_upHorzScale3    ->SetOrthoOffset(fPixBottomOffset);
	m_upSignalControl1->SetVertCoordFreq(&m_vertCoordFreq);
	m_upSignalControl2->SetVertCoordVolt(&m_vertCoordVolt1);
	m_upSignalControl2->SetVertCoordFreq(nullptr);

	int iNrOfTiles { 1 };
	if (m_design == DESIGN::STACKED)
		++iNrOfTiles;
	if (m_bPreview)
		++iNrOfTiles;
	PIXEL const pixTileHeight    { height / iNrOfTiles };
	PIXEL const pixControlHeight { pixTileHeight - H_SCALE_HEIGHT };
	PIXEL const pixControlWidth  { width - V_SCALE_WIDTH };

	if (m_design == DESIGN::INTEGRATED)
	{
		m_upSignalControl1->SetVertCoordVolt(&m_vertCoordVolt1);
		m_upVertScaleVolt1->SetOrthoOffset(0._fPixel);
		m_upVertScaleVolt1->SetOrientation(false);

		m_upVertScaleVolt1->Move(pixControlWidth, 0_PIXEL, V_SCALE_WIDTH, pixControlHeight, true);
		m_upHorzScale1    ->Move(V_SCALE_WIDTH, pixControlHeight, pixControlWidth - V_SCALE_WIDTH, H_SCALE_HEIGHT, true);

		m_upSignalControl2->Show(false);
		m_upHorzScale2    ->Show(false);
	}
	else
	{
		m_upSignalControl1->SetVertCoordVolt(nullptr);
		m_upVertScaleVolt1->SetOrthoOffset(fPixLeftOffset);
		m_upVertScaleVolt1->SetOrientation(true);

		m_upHorzScale1    ->Move(V_SCALE_WIDTH, pixControlHeight,                  pixControlWidth, H_SCALE_HEIGHT,   true);
		m_upSignalControl2->Move(V_SCALE_WIDTH, pixTileHeight,                     pixControlWidth, pixControlHeight, true);
		m_upVertScaleVolt1->Move(      0_PIXEL, pixTileHeight,                     V_SCALE_WIDTH,   pixControlHeight, true);
		m_upHorzScale2    ->Move(V_SCALE_WIDTH, pixTileHeight + pixControlHeight,  pixControlWidth, H_SCALE_HEIGHT,    true);

		m_upSignalControl2->Show(true);
		m_upHorzScale2    ->Show(true);
	}
	
	if (m_bPreview)
	{
		m_upVertScaleVolt2->SetOrthoOffset(fPixLeftOffset);
		m_upVertScaleVolt2->SetOrientation(true);

		m_upSignalPreview ->Move(V_SCALE_WIDTH, height - pixTileHeight,  width - V_SCALE_WIDTH, pixControlHeight, true);
		m_upVertScaleVolt2->Move(0_PIXEL,       height - pixTileHeight,          V_SCALE_WIDTH, pixControlHeight, true);
		m_upHorzScale3    ->Move(V_SCALE_WIDTH, height - H_SCALE_HEIGHT, width - V_SCALE_WIDTH, H_SCALE_HEIGHT,   true);

		m_upSignalPreview->Show(true);
		m_upHorzScale3   ->Show(true);
	}

	m_upSignalControl1->Move(V_SCALE_WIDTH, 0_PIXEL, pixControlWidth, pixControlHeight, true);
	m_upVertScaleFreq ->Move(      0_PIXEL, 0_PIXEL, V_SCALE_WIDTH,   pixControlHeight, true);

	PIXEL pixHorzPos { (width - STIMULUS_BUTTON_WIDTH) / 2 };
	::SetWindowPos 
	(
		m_hStimulusButton,
		HWND_TOP,
		pixHorzPos.GetValue(), 10, 0, 0,
		SWP_NOSIZE
	);

	::SetWindowPos 
	(
		m_upArrowButton->GetWindowHandle(),
		HWND_TOP,
		0, pixControlHeight.GetValue(), 0, 0,
		SWP_NOSIZE
	);
}
