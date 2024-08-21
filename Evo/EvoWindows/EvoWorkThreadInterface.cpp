// EvoWorkThreadInterface.cpp
//
// EvoWindows

#include "GridPoint24.h"
import EvolutionTypes;
#include "EvolutionCore.h"
#include "EvoReadBuffer.h"
#include "EvoHistorySysGlue.h"
#include "win32_script.h"
#include "win32_EvoWorkThread.h"
#include "win32_EvoWorkThreadInterface.h"

import Debug;
import GridPOI;
import Resource;

using std::wostream;
using std::wcout;
using std::endl;

EvoWorkThreadInterface::EvoWorkThreadInterface() :
	m_pEvoWorkThread(nullptr)
{ }

EvoWorkThreadInterface::~EvoWorkThreadInterface()
{
	delete m_pEvoWorkThread;
	m_pEvoWorkThread = nullptr;
}

void EvoWorkThreadInterface::Start
(
	HWND                const hwndApplication,
    ColorManager      * const pColorManager,
	ActionTimer       * const pActionTimer,
    EventInterface    * const pEvent,
	Delay             * const pDelay,
	ObserverInterface * const pObserver, 
    EvoHistorySysGlue * const pEvoHistGlue
)
{
	m_pEvoWorkThread = new EvoWorkThread
	(
		hwndApplication, 
		pColorManager, 
		pActionTimer,
		pEvent, 
		pDelay, 
		pObserver, 
		pEvoHistGlue, 
		this
	);

	WorkThreadInterface::Start(m_pEvoWorkThread);
}

void EvoWorkThreadInterface::PostBenchmark(int const iNrOfGenerations)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << iNrOfGenerations <<endl;
    WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::BENCHMARK), 0, iNrOfGenerations);
}

void EvoWorkThreadInterface::PostDoEdit(GridPoint const gp)
{
    if (IsInGrid(gp))
    {
        if (IsTraceOn())
            TraceStream() << __func__ << L" " << gp << endl;
        WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::DO_EDIT), gp.GetXvalue(), gp.GetYvalue());
    }
}

void EvoWorkThreadInterface::PostSetBrushIntensity(PERCENT const intensity)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << intensity.GetValue() << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_INTENSITY), intensity.GetValue(), 0);
}

void EvoWorkThreadInterface::PostSetBrushRadius(GridCoord const radius)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << radius.GetValue() << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_RADIUS), radius.GetValue(), 0);
}

void EvoWorkThreadInterface::PostSetBrushMode(tBrushMode const mode)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << GetBrushModeName(mode) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_MODE), static_cast<WPARAM>(mode), 0);
}

void EvoWorkThreadInterface::PostSetBrushShape(tShape const shape)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << GetShapeName(shape) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_SHAPE), static_cast<WPARAM>(shape), 0);
}

void EvoWorkThreadInterface::PostSetBrushManipulator(tManipulator const op)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" tManipulator::" << GetManipulatorName(op) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_OPERATOR), static_cast<WPARAM>(op), 0);
}

void EvoWorkThreadInterface::PostSetColor(COLORREF const col, tColorObject const obj, Strategy::Id const strat)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << GetColorObjectName(obj) << endl;
	switch (obj)
	{
	case tColorObject::individual:
	    WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_STRATEGY_COLOR), static_cast<WPARAM>(strat), static_cast<LPARAM>(col));
		break;
	case tColorObject::selection:
	    WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_SELECTION_COLOR), 0, static_cast<LPARAM>(col));
		break;
	case tColorObject::highlight:
	    WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_HIGHLIGHT_COLOR), 0, static_cast<LPARAM>(col));
		break;
	default:
		Assert(false);
	}
}

void EvoWorkThreadInterface::PostSetPOI(GridPoint const gp)
{
	if (IsInGrid(gp))
	{
		if (IsTraceOn())
			TraceStream() << __func__ << L" " << gp << endl;

		GridPOI::SetPoi(gp);

		Continue();     // trigger worker thread if waiting on POI event
	}
}

void EvoWorkThreadInterface::PostGotoOrigin(GridPoint const gp)
{
	if (IsTraceOn())
		TraceStream() << __func__ << L" " << gp << endl;
	WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::GOTO_ORIGIN), gp.GetXvalue(), gp.GetYvalue());
}

void EvoWorkThreadInterface::PostGotoDeath(GridPoint const gp)
{
	if (IsTraceOn())
		TraceStream() << __func__ << L" " << gp << endl;
	WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::GOTO_DEATH), gp.GetXvalue(), gp.GetYvalue());
}
