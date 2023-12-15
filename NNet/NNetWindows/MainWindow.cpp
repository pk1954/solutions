// MainWindow.cpp
//
// NNetWindows

module;

#include "bit"
#include <vector>
#include <optional>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:MainWindow;

import ActionTimer;
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
import Win32_Util;
import Win32_Util_Resource;
import Win32_Controls;
import NNetPreferences;
import ColorLUT;
import :NNetController;
import :MainScales;

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
using std::optional;
using std::nullopt;
using std::vector;

void MainWindow::Start
(
	HWND          const   hwndApp, 
	bool          const   bShowRefreshRateDialog,
	fPixel        const   fPixBeaconLimit,
	NNetController      & controller,
	Observable          & cursorObservable,
	Observable          & coordObservable,  
	Observable          & pStaticModelObservable,
	ActionTimer * const   pActionTimer,
	MonitorWindow const * pMonitorWindow
)
{
	m_pStaticModelObservable = &pStaticModelObservable;
	NNetWindow::Start
	(
		hwndApp, 
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		bShowRefreshRateDialog,
		fPixBeaconLimit,
		controller,
		pMonitorWindow
	);
	ShowRefreshRateDlg(bShowRefreshRateDialog);
	m_pCursorPosObservable = & cursorObservable;
	m_pCoordObservable     = & coordObservable;
	m_pDisplayTimer        = pActionTimer;
	m_selectionMenu.Start(GetWindowHandle());
	m_mainScales.Start(this, GetCoord(), coordObservable);
}

void MainWindow::Stop()
{
	Reset();
	m_selectionMenu.Stop();
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
		{
			appendMenu(hPopupMenu, IDD_NEW_IO_LINE_PAIR);
			appendMenu(hPopupMenu, IDD_ADD_EEG_SENSOR);
		}
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
	m_pCoordObservable->NotifyAll(false);
	return true;
}

bool MainWindow::OnMove(PIXEL const pixPosX, PIXEL const pixPosY)
{
	m_pStaticModelObservable->NotifyAll(false);
	return NNetWindow::OnMove(pixPosX, pixPosY);
};

bool MainWindow::connectionAllowed()
{
	return ConnectionType::ct_none != 
		   m_pNMRI->ConnectionResult(m_nobIdHighlighted, m_nobIdTarget);
}

bool MainWindow::setTargetNob(MicroMeterPnt const& umCrsrPos)
{
	m_nobIdTarget = m_pNMRI->FindNobAt
	(
		umCrsrPos,
		[this](Nob const& s) { return m_pNMRI->IsConnectionCandidate(m_nobIdHighlighted, s.GetId()); }
	);
	return false;
}

bool MainWindow::crsrInScanArea(MicroMeterPnt const &umCrsrPos)
{
	return NNetPreferences::ScanAreaVisible() && m_pNMRI->GetScanAreaRect().Includes(umCrsrPos);
}

void MainWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if (m_pCursorPosObservable)
		m_pCursorPosObservable->NotifyAll(false);

	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam(lParam) };
	fPixelPoint   const fPixCrsr  { Convert2fPixelPoint(ptCrsr) };
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2logUnitPntPos(fPixCrsr) };

	if (wParam == 0)   // no mouse buttons or special keyboard keys pressed
	{
		ClearPtLast();                      // make m_ptLast invalid
		if (m_pNMRI->ModelLocked())    // no edit operations allowed
			return;
		if (setScanAreaHandle(umCrsrPos))
			return;
		if (setHighlightedNob(umCrsrPos))
			return;
		if (setHighlightedSensor(umCrsrPos))
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

	if (!(wParam & MK_LBUTTON))        // Left mouse button
		return;

	MicroMeterPnt const umLastPos { GetCoordC().Transform2logUnitPntPos(ptLast) };
	m_umDelta = umCrsrPos - umLastPos;
	if (m_umDelta.IsZero())
		return;

	if (wParam & MK_CONTROL)   // rotate
	{
		if (m_pNMRI->ModelLocked())    // no edit operations allowed
			return;
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
		SetScanAreaCmd::Push(*m_scanAreaHandleSelected, m_umDelta);
	}
	else if (crsrInScanArea(umCrsrPos))
	{
		if (!m_pNMRI->ModelLocked())    // no edit operations allowed if model is locked
			SetScanAreaCmd::Push(m_pNMRI->GetScanAreaRect() + m_umDelta);
	}
	else if (IsDefined(m_nobIdHighlighted))    // move single nob
	{
		MoveNobCommand::Push(m_nobIdHighlighted, m_umDelta);
		setTargetNob(umCrsrPos);
	}
	else if (m_sensorIdSelected.IsNotNull())
	{
		MoveSensorCmd::Push(m_sensorIdSelected, m_umDelta);
	}
	else if (m_pNMRI->IsAnySignalSelected())
	{
		m_pMonitorWindow->MoveHighlightedSignal(ptCrsr.GetY() - ptLast.GetY());
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
	NobId         const idNob     { m_pNMRI->FindNobAt(umCrsrPos) };
	if (IsUndefined(idNob))
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
	PixelPoint  const ptCrsr   { GetCrsrPosFromLparam(lParam) };
	fPixelPoint const fPixCrsr { Convert2fPixelPoint(ptCrsr) };
	return getSigGenId(fPixCrsr);
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
		ConnectCreateSynapseCmd::Push(idSrc, idDst);  // case 2
		break;
	case ct_neuron:
		Connect2NeuronCommand::Push(idSrc, idDst);    // case 3
		break;
	case ct_knot:
		PlugIoLinesCmd::Push(idSrc, idDst);              // case 4/5
		break;
	case ct_connector:
		Add2ConnectorCmd::Push(idSrc, idDst);     // case 12/13
		break;
	case ct_plugConnectors:
		PlugIoConnectorsCmd::Push(idSrc, idDst);         // case 6
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
	else if (m_pNMRI->IsAnySignalSelected()) 
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

void MainWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
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
	CoordAnimationCmd::Push(GetCoord(), coordTarget);
}

void MainWindow::OnPaint()
{
	m_pDisplayTimer->TimerStart();
	NNetWindow::OnPaint();
	m_pDisplayTimer->TimerStop();
}

/////////////////////// local functions ////////////////////////////////

void MainWindow::PaintGraphics()
{
	PixelRect   const  pixRect         { GetClPixelRect() };
	DrawContext const& context         { GetDrawContextC() };
	Sensor      const* pSensorSelected { m_pNMRI->GetSensor(m_sensorIdSelected) };

	m_mainScales.Paint(*m_upGraphics.get());

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
	{                                                                    // that it is in foreground
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

	if (NNetPreferences::ScanAreaVisible())
	{
		DrawScanArea();
		ScanImage const * const pScanImage
		{
			NNetPreferences::ApplyMedianFilter()
			? m_pNMRI->GetFilteredImageC()
			: m_pNMRI->GetScanImageC()
		};
		if (pScanImage)
		{
			drawScanImage(*pScanImage);
		}
		else
		{
			drawScanRaster();
			drawScanAreaHandles();
		}
	}
}

void MainWindow::drawScanImage(ScanImage const &scanImage) const
{
	Raster const &raster { m_pNMRI->GetScanRaster() };
	RasterPoint   rpRun;
	for (rpRun.m_y = 0; rpRun.m_y < raster.RasterHeight(); ++rpRun.m_y)
	{
		UnitVector<mV> const& imageRow { scanImage.GetRow(rpRun.m_y) };
		for (rpRun.m_x = 0; rpRun.m_x < raster.RasterWidth(); ++rpRun.m_x)
		{
			MicroMeterRect const umRect { raster.GetPointRect(rpRun) };
			mV             const mv     { imageRow.m_vector.at(rpRun.m_x)};
			if (mv.IsNotNull())
			{
				float f = mv.GetValue();
				int   iLutPos = static_cast<int>(f * 256.0f);
				if (iLutPos > 255)
					iLutPos = 255;
				GetDrawContextC().FillRectangle(umRect, m_lut.Get(iLutPos));
			}
		}
	}
}

void MainWindow::drawScanRaster()
{
	MicroMeter const umRes { m_pNMRI->GetScanResolution() };

	if (GetCoordC().Transform2fPixel(umRes) >= 8.0_fPixel)
	{
		Color          const colLine { NNetColors::SCAN_AREA_HANDLE };
		MicroMeterRect const umRect  { m_pNMRI->GetScanAreaRect() };
		RasterPoint    const rSize   { m_pNMRI->GetScanAreaSize() };
		MicroMeter     const umYend  { umRect.GetTop()  + umRes * Cast2Float(rSize.m_y) };
		MicroMeter     const umXend  { umRect.GetLeft() + umRes * Cast2Float(rSize.m_x) };
		for (int x = 0; x <= rSize.m_x; ++x)
		{
			MicroMeter const umX { umRect.GetLeft() + umRes * Cast2Float(x) };
			GetDrawContextC().DrawLine
			(
				MicroMeterPnt(umX, umRect.GetTop()),
				MicroMeterPnt(umX, umYend),
				0.0_MicroMeter,
				colLine
			);
		}
		for (int y = 0; y <= rSize.m_y; ++y)
		{
			MicroMeter const umY { umRect.GetTop() + umRes * Cast2Float(y) };
			GetDrawContextC().DrawLine
			(
				MicroMeterPnt(umRect.GetLeft(), umY),
				MicroMeterPnt(umXend, umY),
				0.0_MicroMeter,
				colLine
			);
		}
	}
}

void MainWindow::drawScanAreaHandles()
{
	MicroMeter const umHandleSize { getScanAreaHandleSize() };
	Apply2CardPoints
	(
		[this, umHandleSize](CardPoint const cp)
		{
			MicroMeterPnt  const pos    { m_pNMRI->GetScanAreaRect().CardPntPos(cp) };
			MicroMeterRect const umRect { RectFromCenterAndExtension<MicroMeter>(pos, umHandleSize) };
			Color          const col 
			{ 
				(m_scanAreaHandleSelected.has_value() && (cp == m_scanAreaHandleSelected.value()))
				? NNetColors::INT_SELECTED 
				: NNetColors::SCAN_AREA_HANDLE
			};
			GetDrawContextC().DrawTranspRect(umRect, col);
		}
	);
}

MicroMeter MainWindow::getScanAreaHandleSize()
{
	fPixel const HANDLE_SIZE { 8.0_fPixel };
	return GetCoordC().Transform2logUnit(HANDLE_SIZE);
}

bool MainWindow::setScanAreaHandle(MicroMeterPnt const& umCrsrPos)
{
	MicroMeter    const umSize            { getScanAreaHandleSize() };
	optional<CardPoint> scanAreaHandleNew { nullopt };
	Apply2CardPoints
	(
		[this, umSize, &umCrsrPos, &scanAreaHandleNew](CardPoint const cardPnt)
		{ 
			MicroMeterPnt  const pos          { m_pNMRI->GetScanAreaRect().CardPntPos(cardPnt) };
			MicroMeterRect const umRectHandle { RectFromCenterAndExtension<MicroMeter>(pos, umSize) };
			if (umRectHandle.Includes(umCrsrPos))
				scanAreaHandleNew = cardPnt;
		}
	);
	if (scanAreaHandleNew != m_scanAreaHandleSelected)
	{
		m_scanAreaHandleSelected = scanAreaHandleNew;
		Notify(false);
	}
	return m_scanAreaHandleSelected.has_value();
}

void MainWindow::drawInputCable(InputLine const& inputLine) const
{
	UPSigGenList const& list     { m_pNMRI->GetSigGenList() };
	SigGenId     const  idSigGen { list.GetSigGenId(*inputLine.GetSigGenC()) };
	bool         const  bActive  { list.IsSelected(idSigGen) };
	switch (NNetPreferences::InputCablesVisibility())
	{
		using enum NNetPreferences::tInputCablesVisibility;
		case all:                                              break;
		case nonStd: if (IsStandardSigGenId(idSigGen)) return; break;
		case active: if (!bActive)                     return; break;
		case none:                                	   return;
	}
	D2D_driver                 & graphics { *m_upGraphics.get() };
	Uniform2D<MicroMeter> const& coord    { m_context.GetCoordC() };
	ID2D1SolidColorBrush* const  pBrush
	{
		(IsHighlighted(inputLine) || bActive)
		? m_pBrushSensorSelected
		: m_pBrushSensorNormal
	};
	list.DrawInputCable
	(
		graphics, 
		coord, 
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
	m_pNMRI->SelectSignal(idSignal);
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
