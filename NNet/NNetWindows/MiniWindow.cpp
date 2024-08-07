// MiniWindow.cpp
//
// NNetWindows

module;

#include <Windows.h>
#include "Resource.h"

module NNetWin32:MiniWindow;

import HiResTimer;

import Types;
import DrawContext;
import NNetModel;
import NNetPreferences;
import :MainWindow;
import :NNetCommandHandler;
import :MainWindow;
import :NNetWindow;

using std::wcout;
using std::endl;

void MiniWindow::Start
(
	HWND         const  hwndParent, 
	bool         const  bShowRefreshRateDialog,
	NNetCommandHandler &controller
)
{
	NNetWindow::Start
	(
		hwndParent, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		bShowRefreshRateDialog,
		controller,
		nullptr
	);
}

void MiniWindow::ObservedNNetWindow(MainWindow * const pNNetWin)	
{ 
	m_pObservedNNetWindow = pNNetWin;
	GetDrawContext().SetNoColors(true);
}

void MiniWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if (! m_pObservedNNetWindow)
		return; 

	if (wParam & MK_LBUTTON)       	// Left mouse button: move or edit action
	{
		PixelPoint const ptCrsr { GetCrsrPosFromLparam(lParam) };  // screen coordinates
		if (GetPtLast().IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			PixelPoint    const pixDelta { ptCrsr - GetPtLast() };
			MicroMeterPnt const umDelta  { GetCoordC().Transform2logUnitPntSize(pixDelta) }; 
			m_pObservedNNetWindow->NNetMove(-umDelta);       // move the observed window in opposite direction
		}
		SetPtLast(ptCrsr);
	}
	else
	{
		ClearPtLast();   // make m_ptLast invalid
	}
}

void MiniWindow::Trigger(bool const bImmediately)
{
	if (m_pObservedNNetWindow)
	{
		float          const EXTRA_SPACE_FACTOR { 1.2f };                               // give 20% more space (looks better)
		MicroMeterRect const umRectMain  { m_pObservedNNetWindow->GetViewRect() };      // current position of main window view 
		MicroMeterRect const umRectModel { m_pNMRI->GetUPNobsC().CalcEnclosingRect() }; // current extension of model
		MicroMeterRect       umRectShow  { Union(umRectMain, umRectModel) };            // all this should be visible  

		if (NNetPreferences::ScanAreaVisible())
			umRectShow = Union(umRectShow, m_pNMRI->GetScanArea());                     // all this should be visible  

		umRectShow.ScaleRect(NEURON_RADIUS);
		MicroMeter const umPixelSizeTarget 
		{ 
			GetCoord().ComputeZoom(umRectShow, GetClRectSize(), EXTRA_SPACE_FACTOR)   //TODO:  use CenterAndZoomRect?
		};
		GetCoord().Zoom(umPixelSizeTarget);
		GetCoord().Center(umRectShow.GetCenter(), Convert2fPixelPoint(GetClRectCenter()));

		NNetWindow::Trigger(false);
	}
}

void MiniWindow::PaintGraphics()
{
	if (m_pObservedNNetWindow)
	{
		DrawContext const& context { GetDrawContextC() };
		DrawExteriorInRect(GetClPixelRect(), [](Nob const &) { return true; }); 
		context.DrawTranspRect(m_pObservedNNetWindow->GetViewRect(), NNetColors::POSITION_RECT);
		if (NNetPreferences::ScanAreaVisible())
			m_pNMRI->DrawScanAreaBackground(context);
	}
}