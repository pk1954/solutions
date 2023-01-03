// Win32_MainWindow.cpp
//
// NNetWindows

module;

#include <unordered_map>
#include <iostream>
#include <string>
#include <Windows.h>
#include "Resource.h"

module MainWindow;

import ActionTimer;
import Commands;
import CreateForkCommand;
import DrawContext;
import ExtendInputLineCmd;
import ExtendOutputLineCmd;
import FatalError;
import NewIoLinePairCmd;
import NNetController;
import NNetModel;
import NNetModelCommands;
import MonitorWindow;
import MoveNobCommand;
import MoveSensorCmd;
import Observable;
import Preferences;
import RootWindow;
import SelectAllConnectedCmd;
import Types;
import Uniform2D;
import Win32_Util;
import Win32_Util_Resource;

using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::to_wstring;
using std::wcout;
using std::endl;

void MainWindow::Start
(
	HWND          const hwndApp, 
	bool          const bShowRefreshRateDialog,
	fPixel        const fPixBeaconLimit,
	Preferences       & preferences,
	NNetController    & controller,
	NNetModelCommands & modelCommands,
	Observable        & cursorObservable,
	Observable        & coordObservable,  
	ActionTimer * const pActionTimer
)
{
	NNetWindow::Start
	(
		hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		bShowRefreshRateDialog,
		fPixBeaconLimit,
		controller
	);
	ShowRefreshRateDlg(bShowRefreshRateDialog);
	m_pPreferences         = & preferences;
	m_pModelCommands       = & modelCommands;
	m_pCursorPosObservable = & cursorObservable;
	m_pCoordObservable     = & coordObservable;
	m_pDisplayTimer        = pActionTimer;
}

void MainWindow::Stop()
{
	Reset();
	NNetWindow::Stop();
}

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
		{ IDD_ARROWS_OFF,             L"Arrows off"                     },
		{ IDD_ARROWS_ON,              L"Arrows on"                      },
		{ IDD_ATTACH_SIG_GEN_TO_LINE, L"Attach active signal generator" },
		{ IDD_ATTACH_SIG_GEN_TO_CONN, L"Attach active signal generator" },
		{ IDM_COPY_SELECTION,         L"Copy selection"                 },
		{ IDM_DELETE_SELECTION,       L"Delete selected objects"        },
		{ IDD_DELETE_NOB,             L"Delete"                         },
		{ IDD_DELETE_EEG_SENSOR,      L"Delete EEG sensor"              },
		{ IDD_DISC_IOCONNECTOR,       L"Disconnect"                     },
		{ IDD_SPLIT_NEURON,           L"Split (make I/O neurons)"       },
		{ IDD_INSERT_KNOT,            L"Insert knot"                    },
		{ IDD_INSERT_NEURON,          L"Insert neuron"                  },
		{ IDD_NEW_IO_LINE_PAIR,       L"New IO-line pair"  	            },
		{ IDM_SELECT,                 L"Select"                         },
		{ IDM_DESELECT,               L"Deselect"                       },
		{ IDD_STOP_ON_TRIGGER,        L"Stop on trigger on/off"         },
		{ IDD_EMPHASIZE,              L"Feedback line on/off"           }
	};
	AppendMenu(hPopupMenu, MF_STRING, idCommand, mapCommands.at(idCommand));
}

LPARAM MainWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	if (m_pNMRI->AnyNobsSelected())
	{
		appendMenu(hPopupMenu, IDM_DESELECT);
		appendMenu(hPopupMenu, IDM_COPY_SELECTION);
		appendMenu(hPopupMenu, IDM_DELETE_SELECTION);
	}
	else if (IsDefined(m_nobIdHighlighted))
	{
		m_pNMRI->GetConstNob(m_nobIdHighlighted)->AppendMenuItems
		(
			[hPopupMenu](int const id){ appendMenu(hPopupMenu, id); }
		);
		if ( m_pNMRI->IsPipe(m_nobIdHighlighted) )
		{
			appendMenu(hPopupMenu, IDD_EMPHASIZE);  
			appendMenu(hPopupMenu, m_pPreferences->ArrowsVisible() ? IDD_ARROWS_OFF : IDD_ARROWS_ON);  
		}
		else if ( m_pNMRI->IsInputLine(m_nobIdHighlighted) )
		{
			if (m_pNMRI->GetSigGenSelectedC() != m_pNMRI->GetSigGenC(m_nobIdHighlighted))
				appendMenu(hPopupMenu, IDD_ATTACH_SIG_GEN_TO_LINE);  
		}
		else if ( m_pNMRI->IsInputConnector(m_nobIdHighlighted) )
		{
			if (m_pNMRI->GetSigGenSelectedC() != m_pNMRI->GetSigGenC(m_nobIdHighlighted))
				appendMenu(hPopupMenu, IDD_ATTACH_SIG_GEN_TO_CONN);  
		}
	}
	else  // nothing selected, cursor on background
	{
		if (m_pNMRI->IsAnySensorSelected())
			appendMenu(hPopupMenu, IDD_DELETE_EEG_SENSOR );
		else
		{
			appendMenu(hPopupMenu, IDD_NEW_IO_LINE_PAIR);
			appendMenu(hPopupMenu, IDD_ADD_EEG_SENSOR);
		}
	}

	return static_cast<LPARAM>(m_nobIdHighlighted.GetValue()); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPnt MainWindow::GetCursorPos() const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition() };
	return IsInClientRect(pixPoint)
		? GetCoordC().Transform2logUnitPntPos(pixPoint)
		: NP_NULL;
}

void MainWindow::AnimateArrows()
{
	MicroMeter oldVal { m_arrowSize };
	MicroMeter umTarget = m_pPreferences->ArrowsVisible() ? STD_ARROW_SIZE : 0._MicroMeter;
	if (umTarget != oldVal)
		m_pModelCommands->AnimateArrows(m_arrowSize, umTarget);
}

void MainWindow::SetSensorPoints() 
{
	m_bShowPnts = m_pPreferences->SensorPointsVisible();
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
	m_pCoordObservable->NotifyAll(false);
	return true;
}

bool MainWindow::connectionAllowed()
{
	return ConnectionType::ct_none != 
		   m_pNMRI->ConnectionResult(m_nobIdHighlighted, m_nobIdTarget);
}

void MainWindow::setTargetNob(MicroMeterPnt const& umCrsrPos)
{
	m_nobIdTarget = m_pNMRI->FindNobAt
	(
		umCrsrPos,
		[this](Nob const & s) { return m_pNMRI->IsConnectionCandidate(m_nobIdHighlighted, s.GetId()); }
	);
}

void MainWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pCursorPosObservable)
		m_pCursorPosObservable->NotifyAll(false);

	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(ptCrsr) };

	if (wParam == 0)   // no mouse buttons or special keyboard keys pressed
	{
		if (!m_bSelectionActive)
		{
			if (!setHighlightedNob(umCrsrPos))
				setHighlightedSensor(umCrsrPos);
		}
		ClearPtLast();                 // make m_ptLast invalid
		return;
	}

	PixelPoint const ptLast { GetPtLast() };
	SetPtLast(ptCrsr);
	if (ptLast.IsNull())
		return;

	if (!(wParam & MK_LBUTTON))        // Left mouse button
		return;

	MicroMeterPnt const umLastPos { GetCoordC().Transform2logUnitPntPos(ptLast) };
	MicroMeterPnt const umDelta   { umCrsrPos - umLastPos };
	if (umDelta.IsZero())
		return;

	if (wParam & MK_CONTROL)   // rotate
	{
		if (m_bSelectionActive)
			m_pModelCommands->RotateSelection(umLastPos, umCrsrPos);
		else if (IsDefined(m_nobIdHighlighted))           
			m_pModelCommands->Rotate(m_nobIdHighlighted, umLastPos, umCrsrPos);
		else 
			m_pModelCommands->RotateModel(umLastPos, umCrsrPos);
	}
	else if (m_bSelectionActive)
	{
		m_pModelCommands->MoveSelection(umDelta);
	}
	else if (IsDefined(m_nobIdHighlighted))    // move single nob
	{
		MoveNobCommand::Push(m_nobIdHighlighted, umDelta);
		setTargetNob(umCrsrPos);
	}
	else if (m_pNMRI->IsAnySensorSelected())
	{
		MoveSensorCmd::Push(m_pNMRI->GetSensorIdSelected(), umDelta);
	}
	else
	{
		NNetMove(ptCrsr - ptLast);     // move view by manipulating coordinate system 
	}
}

void MainWindow::select(NobId const idNob)
{
	SelectAllConnectedCmd::Push(idNob, tBoolOp::opTrue);
	m_bSelectionActive = true;
	m_nobIdHighlighted = NO_NOB;
}

void MainWindow::deselect()
{
	m_pModelCommands->DeselectModule();
	m_bSelectionActive = false;
}

void MainWindow::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(ptCrsr) };
	NobId         const idNob     { m_pNMRI->FindNobAt(umCrsrPos) };
	if (IsUndefined(idNob))
		return;
	if (m_pNMRI->IsSelected(idNob))
	{
		deselect();
		return;
	}
	if (m_bSelectionActive)  // selection active, but other selection desired
		deselect();
	select(idNob);
}

bool MainWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	if (connectionAllowed())
	{
		m_pModelCommands->Connect(m_nobIdHighlighted, m_nobIdTarget);
		Reset();
	}
	return NNetWindow::OnLButtonUp(wParam, lParam);
}

bool MainWindow::OnRButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(ptCrsr) };

	m_umPntSelectionAnchor = umCrsrPos;
	SetFocus();
	return false;
}

void MainWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	static float const ZOOM_FACTOR { 1.1f };

	int   const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool  const bDirection { iDelta > 0 };
	float const fFactor    { bDirection ? 1.0f / ZOOM_FACTOR : ZOOM_FACTOR };

	if (m_pNMRI->AnyNobsSelected())     // operate on selection
	{
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			m_pModelCommands->SizeSelection(fFactor);
		}
	}
	else
	{
		SensorId const sensorId    { m_pNMRI->GetSensorIdSelected() };
		bool     const bSizeSensor { sensorId.IsNotNull() && IsUndefined(m_nobIdHighlighted) };
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			if (bSizeSensor)
			{
				m_pModelCommands->SizeSensor(sensorId, fFactor);
			}
			else
			{
				PixelPoint  const ptCrsr        { GetRelativeCrsrPosition() };  // screen coordinates
				fPixelPoint const fPixPointCrsr { Convert2fPixelPoint(ptCrsr) }; 
				if (GetDrawContext().Zoom(bDirection, fPixPointCrsr))   // Not ok
				{
					if (m_pCoordObservable)
						m_pCoordObservable->NotifyAll(false);
				}
				else
				{
					MessageBeep(MB_ICONWARNING);
				}
			}
		}
	}
	Notify(false); 
}

void MainWindow::centerAndZoomRect
(
	UPNobList::SelMode const mode, 
	float              const fRatioFactor 
)
{
	MicroMeterRect        umRect { m_pNMRI->GetUPNobsC().CalcEnclosingRect(mode) };
	Uniform2D<MicroMeter> coordTarget;
	coordTarget.SetPixelSize  // do not change order!
	(
		GetCoord().ComputeZoom(umRect.ScaleRect(NEURON_RADIUS), GetClRectSize(), fRatioFactor),
		false // do not notify
	);
	coordTarget.SetPixelOffset // do not change order! 
	(
		coordTarget.Transform2fPixelSize(umRect.GetCenter()) -  // SetPixelSize result is used here  
		Convert2fPixelPoint(GetClRectCenter()), 
		false // do not notify
	);
	coordTarget.NotifyAll(true);
	m_pModelCommands->AnimateCoord(GetCoord(), coordTarget);
}

void MainWindow::OnPaint()
{
	m_pDisplayTimer->TimerStart();
 	NNetWindow::OnPaint();
	m_pDisplayTimer->TimerStop();
}

/////////////////////// local functions ////////////////////////////////

void MainWindow::DoPaint()
{
	PixelRect   const         pixRect { GetClPixelRect () };
	DrawContext const &       context { GetDrawContextC() };
	Sensor      const * const pSensor { m_pNMRI->GetSensorSelectedC() };

	DrawSensors();

	if (context.GetPixelSize() <= 5._MicroMeter)
	{
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() && ! n.IsSelected(); }); 
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() &&   n.IsSelected(); }); 
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsPosNob   (); }); // draw PosNobs OVER Pipes
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsIoConnector(); }); 
		if (m_pPreferences->ArrowsVisible())
			DrawArrowsInRect(pixRect, m_arrowSize);
	}

	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() && ! n.IsSelected(); }); 
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() &&   n.IsSelected(); }); 
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsPosNob   (); }); // draw PosNobs OVER Pipes
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsIoConnector(); }); 

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
	{                                                                    // that it is in foreground
		m_pNMRI->DrawExterior(m_nobIdHighlighted, context, tHighlight::highlighted);
		m_pNMRI->DrawInterior(m_nobIdHighlighted, context, tHighlight::highlighted);
	}
	else 
	{
		DrawHighlightedSensor(pSensor);
	}
	if (m_bShowPnts)
	{
		DrawSensorDataPoints(pSensor);
	}
}

bool MainWindow::setHighlightedNob(MicroMeterPnt const& umCrsrPos)
{
	NobId const idHighlight { m_pNMRI->FindNobAt(umCrsrPos) };
	if (idHighlight != m_nobIdHighlighted)
	{
		m_nobIdHighlighted = idHighlight;
		Notify(false);
	}
	return IsDefined(m_nobIdHighlighted);
}

bool MainWindow::setHighlightedSensor(MicroMeterPnt const& umCrsrPos)
{
	SensorId const idSensorOld { m_pNMRI->GetSensorIdSelected() };
	SensorId const idSensorNew { m_pModelCommands->SetHighlightedSensor(umCrsrPos) };
	if (idSensorNew != idSensorOld)
		Notify(false);
	return idSensorNew.IsNotNull();
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
		wcout << Scanner::COMMENT_START << L"command failed, uMsg = " << uMsg << L", wparam =  " << wParam << L", lparam =  " << lParam << endl;
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

bool MainWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	MicroMeterPnt const umPoint { GetCoordC().Transform2logUnitPntPos(pixPoint) };
	switch (int const wmId { LOWORD(wParam) } )
	{

	case IDD_EMPHASIZE:
		m_pModelCommands->ToggleEmphMode(m_nobIdHighlighted);
		break;

	case IDM_DELETE:   // keyboard delete key
		if (IsDefined(m_nobIdHighlighted))
			m_pModelCommands->DeleteNob(m_nobIdHighlighted);
		else if (m_bSelectionActive)
			m_pModelCommands->DeleteSelection();
		return true;

	case IDD_DELETE_NOB:
		m_pModelCommands->DeleteNob(m_nobIdHighlighted);
		break;

	case IDM_SELECT:
		select(m_nobIdHighlighted);
		break;

	case IDM_DESELECT:
		deselect();
		break;

	case IDD_ATTACH_SIG_GEN_TO_LINE:
		m_pModelCommands->AttachSigGen2Line(m_nobIdHighlighted);
		break;

	case IDD_ATTACH_SIG_GEN_TO_CONN:
		m_pModelCommands->AttachSigGen2Conn(m_nobIdHighlighted);
		break;

	case IDM_DELETE_SELECTION:
        m_pModelCommands->DeleteSelection();
        break;

	case IDD_DISC_IOCONNECTOR:
		m_pModelCommands->DiscIoConnector(m_nobIdHighlighted);
		break;

	case IDD_SPLIT_NEURON:
		m_pModelCommands->SplitNeuron(m_nobIdHighlighted);
		break;

	case IDD_INSERT_KNOT:
		m_pModelCommands->InsertKnot(m_nobIdHighlighted, umPoint);
		break;

	case IDD_INSERT_NEURON:
		m_pModelCommands->InsertNeuron(m_nobIdHighlighted, umPoint);
		break;

	case IDD_NEW_IO_LINE_PAIR:
		NewIoLinePairCmd::Push(umPoint);
		break;

	case IDD_CREATE_FORK:         CreateForkCommand::Push(m_nobIdHighlighted, umPoint);	            break; // case 7
	case IDD_CREATE_SYNAPSE:      m_pModelCommands->AddSynapse(m_nobIdHighlighted, umPoint);      	break; // case 8 
	case IDD_ADD_INCOMING2NEURON: m_pModelCommands->AddIncoming2Neuron(m_nobIdHighlighted, umPoint);	break; // case 9
	case IDD_EXTEND_INPUTLINE:    ExtendInputLineCmd ::Push(m_nobIdHighlighted, umPoint);             break; // case 10
	case IDD_EXTEND_OUTPUTLINE:   ExtendOutputLineCmd::Push(m_nobIdHighlighted, umPoint);         	break; // case 11

	case IDD_STOP_ON_TRIGGER:
		m_pModelCommands->ToggleStopOnTrigger(m_nobIdHighlighted);
		break;

	default:
		break;
	}

	return NNetWindow::OnCommand(wParam, lParam, pixPoint);
}
