// Win32_MainWindow.cpp
//
// NNetWindows

module;

#include <unordered_map>
#include <Windows.h>
#include <string>
#include "Resource.h"
#include "Signal.h"
#include "MicroMeterPntVector.h"
#include "NobId.h"
#include "Knot.h"
#include "Neuron.h"
#include "UPNobList.h"
#include "IoConnector.h"
#include "NobException.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "DrawContext.h"
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_rootWindow.h"
#include "win32_monitorWindow.h"
#include "NNetModelReaderInterface.h"

module MainWindow;

import Observable;
import MoreTypes;
import PixelTypes;
import Command;
import ActionTimer;
import FatalError;
import NNetController;
import Preferences;
import Uniform2D;
import FatalError;

using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::to_wstring;

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
	m_nobHighlighted = NO_NOB; 
	setNoTarget(); 
}

void appendMenu(HMENU const hPopupMenu, int const idCommand)
{
	static unordered_map <int, LPCWSTR const> mapCommands =
	{
		{ IDD_ADD_INCOMING2BASEKNOT,   L"Add incoming dendrite"          },
		{ IDD_ADD_INCOMING2PIPE,       L"Add incoming dendrite"          },
		{ IDD_ADD_OUTGOING2BASEKNOT,   L"Add outgoing dendrite"          },
		{ IDD_ADD_OUTGOING2PIPE,       L"Add outgoing dendrite"          },
		{ IDD_ADD_EEG_SENSOR,          L"New EEG sensor" 		         },
//		{ IDM_ALIGN_NOBS,              L"Align selected objects"         },
		{ IDD_ARROWS_OFF,              L"Arrows off"                     },
		{ IDD_ARROWS_ON,               L"Arrows on"                      },
		{ IDD_ATTACH_SIG_GEN_TO_LINE,  L"Attach active signal generator" },
		{ IDD_ATTACH_SIG_GEN_TO_CONN,  L"Attach active signal generator" },
		{ IDD_ATTACH_SIG_GEN_TO_SEL,   L"Attach active signal generator" },
		{ IDM_COPY_SELECTION,          L"Copy selection"                 },
		{ IDM_DELETE_SELECTION,        L"Delete selected objects"        },
		{ IDD_DELETE_NOB,              L"Delete"                         },
		{ IDD_DELETE_EEG_SENSOR,       L"Delete EEG sensor"              },
		{ IDM_DESELECT_ALL,            L"Deselect all"                   },
		{ IDM_DESELECT_NOB,            L"Deselect nob"                   },
		{ IDD_DISC_IOCONNECTOR,        L"Disconnect"                     },
		{ IDD_SPLIT_NEURON,            L"Split (make I/O neurons)"       },
		{ IDD_INSERT_KNOT,             L"Insert knot"                    },
		{ IDD_INSERT_NEURON,           L"Insert neuron"                  },
		{ IDM_MAKE_CONNECTOR,          L"Make connector"                 },
		{ IDD_NEW_IO_LINE_PAIR,        L"New IO-line pair"  	         },
		{ IDM_SELECT_NOB,              L"Select nob"                     },
		{ IDM_SELECT_SUBTREE,          L"Select subtree"                 },
		{ IDD_STOP_ON_TRIGGER,         L"Stop on trigger on/off"         },
		{ IDD_EMPHASIZE,               L"Feedback line on/off"           }
	};
	AppendMenu(hPopupMenu, MF_STRING, idCommand, mapCommands.at(idCommand));
}

LPARAM MainWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	if (m_pNMRI->AnyNobsSelected())
	{
		HMENU hSelectionMenu { Util::PopupMenu(hPopupMenu, L"Selection") };
		appendMenu(hSelectionMenu, IDM_DESELECT_ALL    );
		appendMenu(hSelectionMenu, IDM_COPY_SELECTION  );
		appendMenu(hSelectionMenu, IDM_DELETE_SELECTION);
		appendMenu(hSelectionMenu, IDM_MAKE_CONNECTOR  );
		appendMenu(hSelectionMenu, IDD_ATTACH_SIG_GEN_TO_SEL);  
//		appendMenu(hSelectionMenu, IDM_ALIGN_NOBS      );
	}

	if (IsDefined(m_nobHighlighted))
	{
		m_pNMRI->GetConstNob(m_nobHighlighted)->AppendMenuItems
		(
			[hPopupMenu](int const id){ appendMenu(hPopupMenu, id); }
		);
		if ( m_pNMRI->IsPipe(m_nobHighlighted) )
		{
			appendMenu(hPopupMenu, IDD_EMPHASIZE);  
			appendMenu(hPopupMenu, m_pPreferences->ArrowsVisible() ? IDD_ARROWS_OFF : IDD_ARROWS_ON);  
		}
		else if ( m_pNMRI->IsInputLine(m_nobHighlighted) )
		{
			if (m_pNMRI->GetSigGenSelectedC() != m_pNMRI->GetSigGenC(m_nobHighlighted))
				appendMenu(hPopupMenu, IDD_ATTACH_SIG_GEN_TO_LINE);  
		}
		else if ( m_pNMRI->IsInputConnector(m_nobHighlighted) )
		{
			if (m_pNMRI->GetSigGenSelectedC() != m_pNMRI->GetSigGenC(m_nobHighlighted))
				appendMenu(hPopupMenu, IDD_ATTACH_SIG_GEN_TO_CONN);  
		}
	}
	else  // no nob selected, cursor on background
	{
		if (m_pNMRI->IsAnySensorSelected())
			appendMenu(hPopupMenu, IDD_DELETE_EEG_SENSOR );
		else
		{
			appendMenu(hPopupMenu, IDD_NEW_IO_LINE_PAIR);
			appendMenu(hPopupMenu, IDD_ADD_EEG_SENSOR);
		}
	}

	return static_cast<LPARAM>(m_nobHighlighted.GetValue()); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPnt MainWindow::GetCursorPos() const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition() };
	return IsInClientRect(pixPoint)
		? GetCoordC().Transform2logUnitPntPos(pixPoint)
		: NP_NULL;
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

void MainWindow::setNoTarget()
{
	m_nobTarget = NO_NOB;
	m_connType  = ConnectionType::ct_none;
}

void MainWindow::setTargetNob()
{
	m_nobTarget = m_pNMRI->FindNobAt
	(
		m_pNMRI->GetNobPos(m_nobHighlighted),
		[this](Nob const & s) { return m_pNMRI->IsConnectionCandidate(m_nobHighlighted, s.GetId()); }
	);
	m_connType = m_pNMRI->ConnectionResult(m_nobHighlighted, m_nobTarget);
}

void MainWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pCursorPosObservable)
		m_pCursorPosObservable->NotifyAll(false);

	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(ptCrsr) };

	if (wParam == 0)               // no mouse buttons or special keyboard keys pressed
	{
		m_pModelCommands->SetHighlightedSensor(umCrsrPos);
		setHighlightedNob(umCrsrPos);
		ClearPtLast();             // make m_ptLast invalid
		return;
	}

	PixelPoint const ptLast { GetPtLast() };
	SetPtLast(ptCrsr);
	if (ptLast.IsNull())
		return;

	if (wParam & MK_RBUTTON)        // Right mouse button: selection
	{
		m_rectSelection = MicroMeterRect(m_umPntSelectionAnchor, umCrsrPos);
		Notify(false);
		return;
	}
	
	if (wParam & MK_LBUTTON)        // Left mouse button
	{
		MicroMeterPnt const umLastPos { GetCoordC().Transform2logUnitPntPos(ptLast) };
		MicroMeterPnt const umDelta   { umCrsrPos - umLastPos };
		if (umDelta.IsZero())
			return;
		if (IsDefined(m_nobHighlighted))    // operate on single nob
		{
			if (wParam & MK_CONTROL)
			{
				m_pModelCommands->Rotate(m_nobHighlighted, umLastPos, umCrsrPos);
			}
			else
			{
				m_pModelCommands->MoveNob(m_nobHighlighted, umDelta);
				setTargetNob();
			}
		}
		else if (m_pNMRI->IsAnySensorSelected())
		{   
			SensorId const id { m_pNMRI->GetSensorIdSelected() };
			m_pModelCommands->MoveSensor(id, umDelta);
			Notify(false); 
		}
		else if (wParam & MK_SHIFT)     // operate on selection
		{
			if (m_pNMRI->AnyNobsSelected())
			{
				if (wParam & MK_CONTROL)
				{
					m_pModelCommands->RotateSelection(umLastPos, umCrsrPos);
				}
				else 
				{
					m_pModelCommands->MoveSelection(umDelta);
				}
			}
		}
		else if (wParam & MK_CONTROL)     // rotate model
		{
			m_pModelCommands->RotateModel(umLastPos, umCrsrPos);
		}
		else 
		{
			NNetMove(ptCrsr - ptLast);     // move view by manipulating coordinate system 
		}
	}
	NNetWindow::OnMouseMove(wParam, lParam);
}

void MainWindow::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
	if (IsDefined(m_nobHighlighted) && !m_pNMRI->IsOfType<Knot>(m_nobHighlighted))
	{
		m_pModelCommands->SelectNob(m_nobHighlighted, tBoolOp::opToggle);
	}
}

bool MainWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	if (m_connType != ConnectionType::ct_none)
		SendCommand2Application(IDD_CONNECT, static_cast<LPARAM>(m_connType));
	setNoTarget();
	return NNetWindow::OnLButtonUp(wParam, lParam);
}

bool MainWindow::OnRButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	bool const bSelection { m_rectSelection.IsNotEmpty() };
	if (bSelection)
	{
		m_pModelCommands->SelectNobsInRect(m_rectSelection);
		m_rectSelection.SetZero();
	}
	return bSelection; // let base class handle other cases
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

	if (wParam & MK_SHIFT)     // operate on selection
	{
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			m_pModelCommands->SizeSelection(fFactor);
		}
	}
	else
	{
		PixelPoint    const ptCrsr        { GetRelativeCrsrPosition() };  // screen coordinates
		fPixelPoint   const fPixPointCrsr { Convert2fPixelPoint(ptCrsr) }; 
		MicroMeterPnt const umCrsrPos     { GetCoordC().Transform2logUnitPntPos(fPixPointCrsr) };
		SensorId      const sensorId      { m_pModelCommands->SetHighlightedSensor(umCrsrPos) };
		bool          const bSizeSensor   { sensorId.IsNotNull() && IsUndefined(m_nobHighlighted) };
		for (int iSteps = abs(iDelta); iSteps > 0; --iSteps)
		{
			if (bSizeSensor)
			{
				m_pModelCommands->SizeSensor(sensorId, fFactor);
			}
			else
			{
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

	if (m_rectSelection.IsNotEmpty())
		context.DrawTranspRect(m_rectSelection, NNetColors::SELECTION_RECT);

	DrawSensors();

	if (context.GetPixelSize() <= 5._MicroMeter)
	{
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() && ! n.IsSelected(); }); 
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() &&   n.IsSelected(); }); 
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsBaseKnot (); }); // draw BaseKnots OVER Pipes
		DrawExteriorInRect(pixRect, [](Nob const & n) { return n.IsIoConnector(); }); 
		if (m_pPreferences->ArrowsVisible())
			DrawArrowsInRect(pixRect, m_arrowSize);
	}

	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() && ! n.IsSelected(); }); 
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsPipe() &&   n.IsSelected(); }); 
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsBaseKnot (); }); // draw BaseKnots OVER Pipes
	DrawInteriorInRect(pixRect, [](Nob const & n) { return n.IsIoConnector(); }); 

	if (IsDefined(m_nobTarget)) // draw target nob again to be sure that it is visible
	{
		tHighlight type { (m_connType == ConnectionType::ct_none) ? tHighlight::targetNoFit : tHighlight::targetFit };
		m_pNMRI->DrawExterior(m_nobTarget, context, type);
		m_pNMRI->DrawInterior(m_nobTarget, context, type);
		m_pNMRI->DrawExterior(m_nobHighlighted, context, type);
		m_pNMRI->DrawInterior(m_nobHighlighted, context, type);
	}
	else if (IsDefined(m_nobHighlighted))  // draw highlighted nob again to be sure that it is in foreground
	{
		m_pNMRI->DrawExterior(m_nobHighlighted, context, tHighlight::highlighted);
		m_pNMRI->DrawInterior(m_nobHighlighted, context, tHighlight::highlighted);
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

void MainWindow::setHighlightedNob(MicroMeterPnt const & umCrsrPos)
{
	NobId const idHighlight { m_pNMRI->FindNobAt(umCrsrPos) };
	if (idHighlight != m_nobHighlighted)
	{
		m_nobHighlighted = idHighlight; 
		Notify(false);
	}
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
		m_pNMRI->DUMP();
		wcout << L"highlighted = " << m_nobHighlighted << endl;
		wcout << L"target      = " << m_nobTarget      << endl;
		FatalError::Happened(9, L"Invalid NobId: " + to_wstring(e.m_id.GetValue()));
	}
	return bRes;
}

bool MainWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId { LOWORD(wParam) } )
	{

	case IDM_MAKE_CONNECTOR:
		if (! m_pModelCommands->MakeIoConnector())
			SendCommand2Application(IDX_PLAY_SOUND, reinterpret_cast<LPARAM>(TEXT("NOT_POSSIBLE_SOUND")));
		break;

	default:
		break;
	}

	return NNetWindow::OnCommand(wParam, lParam, pixPoint);
}
