// Win32_MiniWindow.cpp
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>
#include "Resource.h"

module MiniWindow;

import Types;
import MainWindow;
import NNetController;
import MainWindow;
import NNetWindow;
import DrawContext;
import NNetModel;

void MiniWindow::Start
(
	HWND       const hwndParent, 
	bool       const bShowRefreshRateDialog,
	fPixel     const fPixBeaconLimit,
	NNetController & controller
)
{
	NNetWindow::Start
	(
		hwndParent, 
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		bShowRefreshRateDialog,
		fPixBeaconLimit,
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

void MiniWindow::Notify(bool const bImmediately)
{
	if (m_pObservedNNetWindow)
	{
		float            const EXTRA_SPACE_FACTOR { 1.2f };                              // give 20% more space (looks better)
		MicroMeterRect   const umRectMain  { m_pObservedNNetWindow->GetViewRect() };     // current position of main window view 
		MicroMeterRect   const umRectModel { m_pNMRI->GetUPNobsC().CalcEnclosingRect() }; // current extension of model
		MicroMeterRect   const umRectShow  { Union(umRectMain, umRectModel) };           // all this should be visible  
		fPixelPoint      const fpCenter    { Convert2fPixelPoint(GetClRectCenter()) };
		MicroMeterPnt    const umPntTarget { umRectShow.GetCenter() };
		Uniform2D<MicroMeter>& coord       { GetCoord() };
		MicroMeter       const umPixelSizeTarget
		{
			coord.ComputeZoom
			(
				umRectShow.ScaleRect(NEURON_RADIUS), 
				GetClRectSize(),
				EXTRA_SPACE_FACTOR
			)
		};
		coord.Zoom(umPixelSizeTarget);
		coord.Center(umPntTarget, fpCenter);
		NNetWindow::Notify(false);
	}
}

void MiniWindow::DoPaint()
{
	if (m_pObservedNNetWindow)
	{
		DrawSensors();
		DrawExteriorInRect(GetClPixelRect(), [](Nob const &) { return true; }); 
		GetDrawContextC().DrawTranspRect(m_pObservedNNetWindow->GetViewRect(), NNetColors::POSITION_RECT);
		Sensor const* pSensorSelected { m_pNMRI->GetSensorSelectedC() };
		if (pSensorSelected && pSensorSelected->IsMacroSensor())
			DrawHighlightedSensor(static_cast<MacroSensor const*>(pSensorSelected));
	}
}
