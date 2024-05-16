// MainWindow.cpp
//
// NNetWindows

module;

#include "bit"
#include <vector>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <optional>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:MainWindow;

import HiResTimer;
import NNetCommands;
import DrawContext;
import FatalError;
import NNetModel;
import NNetSignals;
import Observable;
import WinManager;
import RootWindow;
import Types;
import Uniform2D;
import Win32_PIXEL;
import Win32_Util;
import Win32_Util_Resource;
import Win32_Controls;
import NNetPreferences;
import :NNetCommandHandler;
import :MainScales;
import :Compute;

using std::bit_cast;
using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::to_wstring;
using std::fixed;
using std::wostringstream;
using std::setprecision;
using std::wcout;
using std::endl;
using std::vector;
using std::optional;

void MainWindow::Start
(
	HWND            const   hwndApp,
	bool            const   bShowRefreshRateDialog,
	NNetCommandHandler    & controller,
	Observable            & cursorObservable,
	Observable            & coordObservable,
	Observable            & pStaticModelObservable,
	Compute         const & compute,      
	HiResTimer    * const pActionTimer,
	MonitorWindow * const pMonitorWindow
)
{
	m_pStaticModelObservable = &pStaticModelObservable;
	NNetWindow::Start
	(
		hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		bShowRefreshRateDialog,
		controller,
		pMonitorWindow
	);
	ShowRefreshRateDlg(bShowRefreshRateDialog);
	m_pCursorPosObservable = & cursorObservable;
	m_pCoordObservable     = & coordObservable;
	m_pCompute             = & compute;
	m_pDisplayTimer        = pActionTimer;
	m_selectionMenu.Start(GetWindowHandle());
	m_mainScales.Start(this, GetCoord(), coordObservable);
}

//void MainWindow::Stop()
//{
//	Reset();
//	m_selectionMenu.Stop();
//	NNetWindow::Stop();
//}

void MainWindow::Reset()
{ 
	m_nobIdHighlighted = NO_NOB;
	m_nobIdTarget = NO_NOB;
}

void appendMenu(HMENU const hPopupMenu, int const idCommand)
{
	static unordered_map <int, LPCWSTR const> mapCommands =
	{
		{ IDD_EXTEND_INPUTLINE,       L"Extend"                         },
		{ IDD_EXTEND_OUTPUTLINE,      L"Extend"                         },
		{ IDD_ADD_INCOMING2NEURON,    L"Add incoming dendrite"          },
		{ IDD_CREATE_SYNAPSE,         L"Create synapse"                 },
		{ IDD_CREATE_FORK,            L"Create fork"                    },
		{ IDD_ADD_EEG_SENSOR,         L"New EEG sensor" 		        },
		{ IDD_ATTACH_SIG_GEN_TO_LINE, L"Attach active signal generator" },
		{ IDD_ATTACH_SIG_GEN_TO_CONN, L"Attach active signal generator" },
		{ IDD_DELETE_NOB,             L"Delete"                         },
		{ IDD_DETACH_NOB,             L"Detach"                         },
		{ IDD_DELETE_EEG_SENSOR,      L"Delete EEG sensor"              },
		{ IDD_ADD_MICRO_SENSOR,       L"Add micro sensor"               },
		{ IDD_DEL_MICRO_SENSOR,       L"Delete micro sensor"            },
		{ IDD_DISC_IOCONNECTOR,       L"Disconnect"                     },
		{ IDD_SPLIT_NEURON,           L"Split (make I/O neurons)"       },
		{ IDD_INSERT_KNOT,            L"Insert knot"                    },
		{ IDD_INSERT_NEURON,          L"Insert neuron"                  },
		{ IDD_NEW_IO_LINE_PAIR,       L"New IO-line pair"  	            },
		{ IDM_SELECT,                 L"Select"                         },
		{ IDD_STOP_ON_TRIGGER,        L"Stop on trigger on/off"         },
		{ IDD_EMPHASIZE,              L"Feedback line on/off"           }
	};
	AppendMenu(hPopupMenu, MF_STRING, idCommand, mapCommands.at(idCommand));
}

LPARAM MainWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	if (m_pNMRI->AnyNobsSelected())
	{
		// no context menu, use selection menu
	}
	else if (IsDefined(m_nobIdHighlighted))
	{
		m_pNMRI->GetConstNob(m_nobIdHighlighted)->AppendMenuItems
		(
			[hPopupMenu](int const id){ appendMenu(hPopupMenu, id); }
		);

		if (m_pNMRI->HasMicroSensor(m_nobIdHighlighted))   
			appendMenu(hPopupMenu, IDD_DEL_MICRO_SENSOR);
		else
			appendMenu(hPopupMenu, IDD_ADD_MICRO_SENSOR);

		if (m_pNMRI->IsInputLine(m_nobIdHighlighted))
		{
			if (m_pNMRI->GetSigGenSelectedC() != m_pNMRI->GetSigGenC(m_nobIdHighlighted))
				appendMenu(hPopupMenu, IDD_ATTACH_SIG_GEN_TO_LINE);  
		}
	}
	else if (IsAddSigGenButton(m_idSigGenUnderCrsr))
	{
		// no context menu
	}
	else if (IsValidSigGenId(m_idSigGenUnderCrsr))
	{
		SignalDesigner::AddSigGenMenu(hPopupMenu, m_idSigGenUnderCrsr);
	}
	else if (m_sensorIdSelected.IsNotNull())
	{
		appendMenu(hPopupMenu, IDD_DELETE_EEG_SENSOR);
	}
	else  // nothing selected, cursor on background
	{
		if (!m_pNMRI->ModelLocked())    // no edit operations allowed if model is locked
			appendMenu(hPopupMenu, IDD_NEW_IO_LINE_PAIR);
		appendMenu(hPopupMenu, IDD_ADD_EEG_SENSOR);
	}
	if (m_pNMRI->GetScanImageC())
	{
		AppendMenu
		(
			hPopupMenu, MF_STRING, IDD_FILTER,
			NNetPreferences::ApplyFilter() ? L"No filter" : L"Mean filter"
		);
	}
	if (NNetPreferences::ScanAreaVisible())
	{
		AppendMenu
		(
			hPopupMenu, MF_STRING, IDD_MODEL_FRONT,
			NNetPreferences::ModelFront() ? L"Model in background" : L"Model in foreground"
		);
	}

	m_mainScales.AppendScaleMenu(hPopupMenu, L"&Scales");

	NNetWindow::AddContextMenuEntries(hPopupMenu);

	return static_cast<LPARAM>(m_nobIdHighlighted.GetValue()); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPnt MainWindow::GetCursorPos() const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition() };
	return IsInClientRect(pixPoint)
		? GetCoordC().Transform2logUnitPntPos(pixPoint)
		: NP_NULL;
}

//void MainWindow::OnSetCursor(WPARAM const wParam, LPARAM const lParam)
//{
//	bool    const keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
//	HCURSOR const hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
//	SetCursor(hCrsr);
//}

bool MainWindow::OnSize(PIXEL const width, PIXEL const height)
{
	NNetWindow::OnSize(width, height);
	m_mainScales.AdjustScales();
	m_pCoordObservable->NotifyAll();
	return true;
}

bool MainWindow::OnMove(PIXEL const pixPosX, PIXEL const pixPosY)
{
	m_pStaticModelObservable->NotifyAll();  //position of sensor cables
	return NNetWindow::OnMove(pixPosX, pixPosY);
};

bool MainWindow::connectionAllowed()
{
	return ConnectionType::ct_none !=  m_pNMRI->ConnectionResult(m_nobIdHighlighted, m_nobIdTarget);
}

bool MainWindow::setScanAreaHandle(MicroMeterPnt const& umCrsrPos)
{
	m_scanAreaHandleSelected = m_pNMRI->SelectScanAreaHandle(GetDrawContextC(), umCrsrPos);
	return m_scanAreaHandleSelected.has_value();
}

void MainWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pCursorPosObservable)
		m_pCursorPosObservable->NotifyAll();

	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };
	fPixelPoint   const fPixCrsr  { Convert2fPixelPoint(ptCrsr) };
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(fPixCrsr) };

	if (wParam == 0)   // no mouse buttons or special keyboard keys pressed
	{
		ClearPtLast();                 // make m_ptLast invalid
		if (setHighlightedSensor(umCrsrPos))
			return;
		if (m_pNMRI->ModelLocked())    // no edit operations allowed
			return;
		if (setScanAreaHandle(umCrsrPos))
			return;
		if (setHighlightedNob(umCrsrPos))
			return;
		if (selectSignalHandle(umCrsrPos))
			return;
		m_idSigGenUnderCrsr = getSigGenId(fPixCrsr);
		return;
	}

	PixelPoint const ptLast { GetPtLast() };
	SetPtLast(ptCrsr);
	if (ptLast.IsNull())
		return;

	if (!(wParam & MK_LBUTTON))    // left mouse button
		return;

	MicroMeterPnt const umLastPos { GetCoordC().Transform2logUnitPntPos(ptLast) };
	m_umDelta = umCrsrPos - umLastPos;
	if (m_umDelta.IsZero())
		return;

	if (m_sensorIdSelected.IsNotNull())
	{
		MoveSensorCmd::Push(m_sensorIdSelected, m_umDelta);
	}
	else if (m_pNMRI->ModelLocked())    // no edit operations allowed
	{
		NNetMove(ptCrsr - ptLast);     // move view by manipulating coordinate system 
	}
	else if (wParam & MK_CONTROL)   // rotate
	{
		if (selectionCommand(wParam))
			RotateSelectionCommand::Push(umLastPos, umCrsrPos);
		else if (IsDefined(m_nobIdHighlighted))           
			RotateNobCommand::Push(m_nobIdHighlighted, umLastPos, umCrsrPos);
		else 
			RotateModelCommand::Push(umLastPos, umCrsrPos);
	}
	else if (selectionCommand(wParam))
	{
		MoveSelectionCommand::Push(m_umDelta);
	}
	else if (m_scanAreaHandleSelected.has_value())    // manipulate selection area
	{
		SetScanAreaCmd::Push(m_scanAreaHandleSelected.value(), m_umDelta);
	}
	else if (IsDefined(m_nobIdHighlighted))    // move single nob
	{
		MoveNobCommand::Push(m_nobIdHighlighted, m_umDelta);
		m_nobIdTarget = m_pNMRI->FindConnectionCandidate(umCrsrPos, m_nobIdHighlighted);
	}
	else if (m_pMonitorWindow->IsAnySignalHighlighted())
	{
		m_pMonitorWindow->MoveHighlightedSignal(ptCrsr.GetY() - ptLast.GetY());
	}
	else if (NNetPreferences::ScanAreaVisible() && m_pNMRI->GetScanArea().Includes(umCrsrPos))
	{
		SetScanAreaCmd::Push(m_pNMRI->GetScanArea() + m_umDelta);
	}
	else
	{
		NNetMove(ptCrsr - ptLast);     // move view by manipulating coordinate system 
	}
}

void MainWindow::select(NobId const idNob)
{
	SelectAllConnectedCmd::Push(idNob);
	m_nobIdHighlighted = NO_NOB;
	m_selectionMenu.Move(GetRelativeCrsrPosition());
}

void MainWindow::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(ptCrsr) };
	NobId         const idNob     { m_pNMRI->FindAnyNobAt(umCrsrPos) };
	if (IsUndefined(idNob))
		return;
	if (m_pNMRI->ModelLocked())    // no edit operations allowed
		return;
	if (m_pNMRI->IsSelected(idNob))
	{
		DeselectModuleCmd::Push();
		return;
	}
	if (m_pNMRI->AnyNobsSelected())  // selection active, but other selection desired
		DeselectModuleCmd::Push();
	select(idNob);
}

SigGenId MainWindow::getSigGenId(fPixelPoint const &fPixCrsr)
{
	fPixel   const fOffset  { m_mainScales.VerticalOffset() };
	SigGenId const idSigGen { m_pNMRI->GetSigGenList().GetSigGenId(fPixCrsr, fOffset) };
	return idSigGen;
}

SigGenId MainWindow::getSigGenId(LPARAM const lParam)
{
	return getSigGenId(GetCrsrPosFromLparamF(lParam));
}

bool MainWindow::selectSigGen(SigGenId const id)
{
	bool bRes { IsValidSigGenId(id) };
	if (bRes)
	{
		m_idSigGenUnderCrsr = id;
		WinManager::SendCommand2App(IDD_SELECT_SIGNAL_GENERATOR, id.GetValue());
	}
	return bRes;
}

void MainWindow::connect(NobId const idSrc, NobId const idDst)
{
	using enum ConnectionType;

	ConnectionType connType { m_pNMRI->ConnectionResult(idSrc, idDst) };
	switch (connType)
	{
	case ct_fork: ConnectCreateForkCmd::Push(idSrc, idDst);  // case 1 
		break;
	case ct_synapse:
		ConnectCreateSynapseCmd::Push(idSrc, idDst); // case 2
		break;
	case ct_neuron:
		Connect2NeuronCommand::Push(idSrc, idDst);   // case 3
		break;
	case ct_knot:
		PlugIoLinesCmd::Push(idSrc, idDst);          // case 4/5
		break;
	case ct_connector:
		Add2ConnectorCmd::Push(idSrc, idDst);        // case 12/13
		break;
	case ct_plugConnectors:
		PlugIoConnectorsCmd::Push(idSrc, idDst);     // case 6
		break;
	default: assert(false);
	}
}

bool MainWindow::OnLButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	SigGenId const idSigGen { getSigGenId(lParam) };
	if (idSigGen == ADD_SIGGEN)
		WinManager::SendCommand2App(IDD_NEW_SIGNAL_GENERATOR);
	else if (!selectSigGen(idSigGen))
		SetCapture();
	return NNetWindow::OnLButtonDown(wParam, lParam);
}

bool MainWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	if (connectionAllowed())
	{
		connect(m_nobIdHighlighted, m_nobIdTarget);
		Reset();
	}
	else if (m_pMonitorWindow->IsAnySignalHighlighted()) 
	{
		m_pMonitorWindow->DropSignal();
	}

	return NNetWindow::OnLButtonUp(wParam, lParam);
}

bool MainWindow::OnRButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	selectSigGen(getSigGenId(lParam));
	SetFocus();
	return false;
}

bool MainWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	static float const ZOOM_FACTOR { 1.1f };

	int   const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool  const bDirection { iDelta > 0 };
	float const fFactor    { bDirection ? 1.0f / ZOOM_FACTOR : ZOOM_FACTOR };

	if (selectionCommand(wParam))     // operate on selection
	{
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			SizeSelectionCmd::Push(fFactor);
		}
	}
	else
	{
		bool const bSizeSensor { m_sensorIdSelected.IsNotNull() && IsUndefined(m_nobIdHighlighted) };
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			if (bSizeSensor)
			{
				SizeSensorCmd::Push(m_sensorIdSelected, fFactor);
			}
			else
			{
				PixelPoint  const ptCrsr        { GetRelativeCrsrPosition() };
				fPixelPoint const fPixPointCrsr { Convert2fPixelPoint(ptCrsr) }; 
				if (GetDrawContext().Zoom(bDirection, fPixPointCrsr))   // Not ok
				{
					if (m_pCoordObservable)
						m_pCoordObservable->NotifyAll();
				}
				else
				{
					MessageBeep(MB_ICONWARNING);
				}
			}
		}
	}
	Notify(false); 
	return true;
}

void MainWindow::centerAndZoomRect
(
	UPNobList::SelMode const mode, 
	float              const fRatioFactor 
)
{
	Uniform2D<MicroMeter> coordTarget;
	MicroMeterRect        umRect { m_pNMRI->GetUPNobsC().CalcEnclosingRect(mode) };
	umRect.ScaleRect(NEURON_RADIUS);
	CenterAndZoomRect(coordTarget, umRect, fRatioFactor);
	CoordAnimationCmd::Push(GetCoord(), coordTarget);
}

void MainWindow::OnPaint()
{
	m_pDisplayTimer->BeforeAction();
	NNetWindow::OnPaint();
	m_pDisplayTimer->AfterAction();
}

/////////////////////// local functions ////////////////////////////////

void MainWindow::PaintGraphics()
{
	m_mainScales.PaintGrid(*m_upGraphics.get());

	if (NNetPreferences::ScanAreaVisible())
	{
		if (NNetPreferences::ModelFront())
		{
			drawScanArea();
			drawModel(m_context );
		}
		else
		{
			drawModel(m_context);
			drawScanArea();
		}
	}
	else
	{
		drawModel(m_context);
	}
}

void MainWindow::drawScanArea()
{
	if (m_pCompute->IsScanRunning())
		m_pNMRI->DrawScanProgress(m_context, m_pCompute->ScanProgress());
	else
		DrawScanAreaAll(m_scanAreaHandleSelected);
}

void MainWindow::drawModel(DrawContext const& context)
{
	PixelRect const  pixRect         { GetClPixelRect() };
	Sensor    const* pSensorSelected { m_pNMRI->GetSensor(m_sensorIdSelected) };

	if (context.GetPixelSize() <= 5._MicroMeter)
	{
		DrawExteriorInRect(pixRect, [](Nob const& n) { return n.IsPipe() && !n.IsSelected(); });
		DrawExteriorInRect(pixRect, [](Nob const& n) { return n.IsPipe() && n.IsSelected(); });
		DrawExteriorInRect(pixRect, [](Nob const& n) { return n.IsPosNob(); }); // draw PosNobs OVER Pipes
		DrawExteriorInRect(pixRect, [](Nob const& n) { return n.IsIoConnector(); });
		if (m_umArrowSize > 0.0_MicroMeter)
			DrawArrowsInRect(pixRect, m_umArrowSize);
	}

	DrawInteriorInRect(pixRect, [](Nob const& n) { return n.IsPipe() && !n.IsSelected(); });
	DrawInteriorInRect(pixRect, [](Nob const& n) { return n.IsPipe() && n.IsSelected(); });
	DrawInteriorInRect(pixRect, [](Nob const& n) { return n.IsPosNob(); }); // draw PosNobs OVER Pipes
	DrawInteriorInRect(pixRect, [](Nob const& n) { return n.IsIoConnector(); });

	if (m_pNMRI->IsValidNobId(m_nobIdTarget)) // draw target nob again to be sure that it is visible
	{
		tHighlight type { connectionAllowed() ? tHighlight::targetFit : tHighlight::targetNoFit };
		m_pNMRI->DrawExterior(m_nobIdTarget, context, type);
		m_pNMRI->DrawInterior(m_nobIdTarget, context, type);
		if (IsDefined(m_nobIdHighlighted))
		{
			m_pNMRI->DrawExterior(m_nobIdHighlighted, context, type);
			m_pNMRI->DrawInterior(m_nobIdHighlighted, context, type);
		}
	}
	else if (m_pNMRI->IsValidNobId(m_nobIdHighlighted))  // draw highlighted nob again to be sure 
	{                                                    // that it is in foreground
		m_pNMRI->DrawExterior(m_nobIdHighlighted, context, tHighlight::highlighted);
		m_pNMRI->DrawInterior(m_nobIdHighlighted, context, tHighlight::highlighted);
	}
	else
	{
		if (pSensorSelected)
			pSensorSelected->Draw(m_context, true);
	}

	DrawSensors();
	m_pNMRI->GetSigGenList().DrawSignalGenerators(*m_upGraphics.get(), m_mainScales.VerticalOffset());
	m_pNMRI->Apply2AllC<InputLine>([this](InputLine const& i) { drawInputCable(i); });

	if (NNetPreferences::m_bSensorPoints.Get() && pSensorSelected)
		pSensorSelected->DrawDataPoints(m_context);

	m_selectionMenu.Show(m_pNMRI->AnyNobsSelected());
}

void MainWindow::drawInputCable(InputLine const& inputLine) const
{
	UPSigGenList const& list     { m_pNMRI->GetSigGenList() };
	SigGenId     const  idSigGen { list.GetSigGenId(*inputLine.GetSigGenC()) };
	bool         const  bActive  { list.IsSelected(idSigGen) };
	switch (NNetPreferences::m_bInputCables.Visibility())
	{
		using enum ShowInputCables::tVisibility;
		case all:                                              break;
		case nonStd: if (IsStandardSigGenId(idSigGen)) return; break;
		case active: if (!bActive)                     return; break;
		case none:                                	   return;
	}
	ID2D1SolidColorBrush* const pBrush
	{
		(IsHighlighted(inputLine) || bActive)
		? m_pBrushSensorSelected
		: m_pBrushSensorNormal
	};
	list.DrawInputCable
	(
		*m_upGraphics.get(), 
		GetCoordC(), 
		m_mainScales.VerticalOffset(), 
		idSigGen, 
		inputLine, 
		pBrush
	);
}

bool MainWindow::selectionCommand(WPARAM const wParam)
{
	return m_pNMRI->AnyNobsSelected() && (wParam & MK_SHIFT);
}

bool MainWindow::setHighlightedNob(MicroMeterPnt const& umCrsrPos)
{
	NobId const idHighlight { m_pNMRI->FindAnyNobAt(umCrsrPos) };
	if (idHighlight != m_nobIdHighlighted)
	{
		m_nobIdHighlighted = idHighlight;
		Notify(false);
	}
	return IsDefined(m_nobIdHighlighted);
}

bool MainWindow::setHighlightedSensor(MicroMeterPnt const& umCrsrPos)
{
	SensorId const idSensorNew { m_pNMRI->GetSensorList().FindSensor(umCrsrPos) };
	if (idSensorNew != m_sensorIdSelected)
	{
		m_sensorIdSelected = idSensorNew;
		Notify(false);
	}
	return idSensorNew.IsNotNull();
}

bool MainWindow::selectSignalHandle(MicroMeterPnt const& umCrsrPos)
{
	SignalId const idSignal { FindSignalHandle(umCrsrPos) };
	m_pMonitorWindow->SetHighlightedSignal(idSignal);
	return idSignal.IsNotNull();
}

bool MainWindow::UserProc
(
	UINT   const uMsg, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	if (uMsg == WM_APP_UI_CALL)
	{ 
		Command::DoCall(wParam, lParam);
		return false;
	}
	bool bRes;
	try
	{
		bRes = NNetWindow::UserProc(uMsg, wParam, lParam); 
	}
	catch (NobException const & e)
	{
		wcout << COMMENT_START << L"command failed, uMsg = " << uMsg << L", wparam =  " << wParam << L", lparam =  " << lParam << endl;
		m_pNMRI->DumpModel(__FILE__, __LINE__);
		wcout << L"highlighted = " << m_nobIdHighlighted << endl;
		wcout << L"target      = " << m_nobIdTarget << endl;
		FatalError::Happened(9, L"Invalid NobId: " + to_wstring(e.m_id.GetValue()));
	}
	return bRes;
}

void MainWindow::CenterModel()
{
	centerAndZoomRect(UPNobList::SelMode::allNobs, 1.2f); // give 20% more space (looks better)
}

void MainWindow::CenterSelection()
{
	if (m_pNMRI->AnyNobsSelected())
		centerAndZoomRect(UPNobList::SelMode::selectedNobs, 2.0f);
}

void MainWindow::OnChar(WPARAM const wParam, LPARAM const lParam)
{
	if ((wParam == VK_SPACE) && (LeftMouseButtonDown()) && IsDefined(m_nobIdHighlighted))
	{
		Nob const* pNob { m_pNMRI->GetConstNob(m_nobIdHighlighted) };
		if (pNob->IsIoLine())
		{
			MicroMeterPnt const umPoint  { pNob->GetPos() };
			MicroMeterPnt const umPntVec { Radian2Vector(pNob->GetDir()).ScaledTo(MICRO_OFFSET) };
			if (pNob->IsInputLine())
			{
				ExtendInputLineCmd::Push(m_nobIdHighlighted, umPoint - umPntVec);
			}
			else
			{
				ExtendOutputLineCmd::Push(m_nobIdHighlighted, umPoint + umPntVec);
			}
			setHighlightedNob(umPoint);
		}
	}
}

bool MainWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	MicroMeterPnt const umPoint { GetCoordC().Transform2logUnitPntPos(pixPoint) };
	switch (int const wmId { LOWORD(wParam) } )
	{

	case IDM_DELETE:   // keyboard delete key
		if (IsDefined(m_nobIdHighlighted))
			DeleteNobCommand::Push(m_nobIdHighlighted);
		else if (m_pNMRI->AnyNobsSelected())
			DeleteSelectionCommand::Push();
		m_nobIdTarget = NO_NOB;
		return true;

	case IDD_DETACH_NOB:
	case IDD_DELETE_NOB:
		DeleteNobCommand::Push(m_nobIdHighlighted);
		m_nobIdTarget = NO_NOB;
		break;

	case IDM_SELECT:
		select(m_nobIdHighlighted);
		break;

	case IDM_ESCAPE:
		m_nobIdTarget = NO_NOB;
		m_nobIdHighlighted = NO_NOB;
		break;

	case IDM_DELETE_SELECTION:
		DeleteSelectionCommand::Push();
		m_nobIdTarget = NO_NOB;
		break;

	case IDD_RENAME_SIGNAL_GENERATOR:		
		RenameSigGenCmd::Dialog(GetWindowHandle());		
		break;

	case IDD_DELETE_SIGNAL_GENERATOR: DeleteSigGenCmd       ::Push();	                                      break;
	case IDM_DESELECT:		          DeselectModuleCmd     ::Push();	                                      break;
	case IDM_COPY_SELECTION:		  AddNobsCommand        ::Push();		                                  break;
	case IDD_EMPHASIZE:		          ToggleEmphModeCmd     ::Push(m_nobIdHighlighted);		                  break;
	case IDD_ATTACH_SIG_GEN_TO_LINE:  AttachSigGen2LineCmd  ::Push(m_nobIdHighlighted);		                  break;
	case IDD_ATTACH_SIG_GEN_TO_CONN:  AttachSigGen2ConCmd   ::Push(m_nobIdHighlighted);		                  break;
	case IDD_DISC_IOCONNECTOR:		  DiscIoConnectorCmd    ::Push(m_nobIdHighlighted);	                      break;
	case IDD_DELETE_EEG_SENSOR:       DelSensorCmd          ::Push(m_sensorIdSelected);		                  break;
	case IDD_SPLIT_NEURON:            SplitNeuronCmd        ::Push(m_nobIdHighlighted);	                      break;
	case IDD_INSERT_KNOT:             InsertKnotCommand     ::Push(m_nobIdHighlighted, umPoint);		      break;
	case IDD_INSERT_NEURON:           InsertNeuronCommand   ::Push(m_nobIdHighlighted, umPoint);              break;
	case IDD_NEW_IO_LINE_PAIR:        NewIoLinePairCmd      ::Push(umPoint);                                  break;
	case IDD_ADD_MICRO_SENSOR:        AddMicroSensorCmd     ::Push(m_nobIdHighlighted, TrackNr(0));	          break;
	case IDD_DEL_MICRO_SENSOR:        DelMicroSensorCmd     ::Push(m_nobIdHighlighted);	                      break;
	case IDD_CREATE_FORK:             CreateForkCommand     ::Push(m_nobIdHighlighted, umPoint);	          break; // case 7
	case IDD_CREATE_SYNAPSE:          CreateSynapseCommand  ::Push(m_nobIdHighlighted, umPoint);              break; // case 8 
	case IDD_ADD_INCOMING2NEURON:     AddPipe2NeuronCmd     ::Push(m_nobIdHighlighted, umPoint - STD_OFFSET); break; // case 9
	case IDD_EXTEND_INPUTLINE:        ExtendInputLineCmd    ::Push(m_nobIdHighlighted, umPoint - STD_OFFSET); break; // case 10
	case IDD_EXTEND_OUTPUTLINE:       ExtendOutputLineCmd   ::Push(m_nobIdHighlighted, umPoint + STD_OFFSET); break; // case 11
	case IDD_STOP_ON_TRIGGER:         ToggleStopOnTriggerCmd::Push(m_nobIdHighlighted);           		      break;
	case IDD_ARROW_ANIMATION:         ArrowAnimationCmd     ::Push(m_umArrowSize, NNetPreferences::m_bArrows.Get(), lParam); break;

	case IDM_SCALE_OFF:
	case IDM_SCALE_ON:
	case IDM_SCALE_GRID:
	case IDD_SCALES_UPDATE:
	case IDD_GRID_UPDATE:
		m_mainScales.HandleCommand(wmId);
		break;

	default:
		return NNetWindow::OnCommand(wParam, lParam, pixPoint);
	}

	return true;
}
