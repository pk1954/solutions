// EvoWorkThread.cpp
//
// EvoWindows

module EvoWorkThread;

import std;
import RunTime;
import EvoHistorySysGlue;
import EventInterface;
import EvoReadBuffer;
import Thread;
import EventInterface;
import ColorManager;
import EvoCoreLib;

EvoWorkThread::EvoWorkThread
(
	HWND                const hwndApplication,
	ColorManager      * const pColorManager,
	EventInterface    * const pEvent,
	ObserverInterface * const pObserver, 
	EvoHistorySysGlue * const pEvoHistorySys,
) :
	WorkThread
	(
		hwndApplication, 
		pActionTimer, 
		pEvent, 
		pObserver, 
		pEvoHistorySys->GetHistorySystem(), 
		nullptr,
		pWorkThread,
		TRUE
	),
	m_pColorManager(pColorManager),
	m_pEvoHistGlue (pEvoHistorySys)
{ 
}

EvoWorkThread::~EvoWorkThread()
{
	m_pColorManager = nullptr;
	m_pEvoHistGlue  = nullptr;
}

void EvoWorkThread::Start
(
	HWND                const hwndApplication,
    ColorManager      * const pColorManager,
    EventInterface    * const pEvent,
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
		pObserver, 
		pEvoHistGlue, 
		this
	);

	WorkThread::Start(m_pEvoWorkThread);
}

void EvoWorkThread::PostBenchmark(int const iNrOfGenerations)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << iNrOfGenerations <<endl;
    WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::BENCHMARK), 0, iNrOfGenerations);
}

void EvoWorkThread::PostDoEdit(GridPoint const gp)
{
    if (IsInGrid(gp))
    {
        if (IsTraceOn())
            TraceStream() << __func__ << L" " << gp << endl;
        WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::DO_EDIT), gp.GetXvalue(), gp.GetYvalue());
    }
}

void EvoWorkThread::PostSetBrushIntensity(PERCENT const intensity)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << intensity.GetValue() << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_INTENSITY), intensity.GetValue(), 0);
}

void EvoWorkThread::PostSetBrushRadius(GridCoord const radius)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << radius.GetValue() << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_RADIUS), radius.GetValue(), 0);
}

void EvoWorkThread::PostSetBrushMode(tBrushMode const mode)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << GetBrushModeName(mode) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_MODE), static_cast<WPARAM>(mode), 0);
}

void EvoWorkThread::PostSetBrushShape(tShape const shape)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" " << GetShapeName(shape) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_SHAPE), static_cast<WPARAM>(shape), 0);
}

void EvoWorkThread::PostSetBrushManipulator(tManipulator const op)
{
    if (IsTraceOn())
        TraceStream() << __func__ << L" tManipulator::" << GetManipulatorName(op) << endl;
    WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::SET_BRUSH_OPERATOR), static_cast<WPARAM>(op), 0);
}

void EvoWorkThread::PostSetColor(COLORREF const col, tColorObject const obj, Strategy::Id const strat)
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

void EvoWorkThread::PostSetPOI(GridPoint const gp)
{
	if (IsInGrid(gp))
	{
		if (IsTraceOn())
			TraceStream() << __func__ << L" " << gp << endl;

		GridPOI::SetPoi(gp);

		Continue();     // trigger worker thread if waiting on POI event
	}
}

void EvoWorkThread::PostGotoOrigin(GridPoint const gp)
{
	if (IsTraceOn())
		TraceStream() << __func__ << L" " << gp << endl;
	WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::GOTO_ORIGIN), gp.GetXvalue(), gp.GetYvalue());
}

void EvoWorkThread::PostGotoDeath(GridPoint const gp)
{
	if (IsTraceOn())
		TraceStream() << __func__ << L" " << gp << endl;
	WorkMessage(FALSE, static_cast<WorkThreadMessage::Id>(EvoWorkThreadMessage::Id::GOTO_DEATH), gp.GetXvalue(), gp.GetYvalue());
}


bool EvoWorkThread::Dispatch(MSG const msg )
{
	switch (static_cast<EvoWorkThreadMessage::Id>(msg.message))
	{

	case EvoWorkThreadMessage::Id::BENCHMARK:
		NGenerationSteps(static_cast<int>(msg.lParam));
		break;

	case EvoWorkThreadMessage::Id::GOTO_ORIGIN:
	case EvoWorkThreadMessage::Id::GOTO_DEATH:
		GotoGeneration
		(
			m_pEvoHistGlue->GetGenWithIndividual
			(
				GridPoint
				(
					static_cast<GridCoord>(Cast2Short(msg.wParam)), 
					static_cast<GridCoord>(Cast2Short(msg.lParam))
				),
				static_cast<EvoWorkThreadMessage::Id>(msg.message) == EvoWorkThreadMessage::Id::GOTO_DEATH 
			)
		);
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_MODE:
		editorCommand(EvoGenerationCmd::Id::editSetBrushMode, msg.wParam);
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_RADIUS:
		editorCommand(EvoGenerationCmd::Id::editSetBrushRadius, msg.wParam);
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_SHAPE:
		editorCommand(EvoGenerationCmd::Id::editSetBrushShape, msg.wParam);
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_OPERATOR:
		editorCommand(EvoGenerationCmd::Id::editSetBrushManipulator, msg.wParam);
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_INTENSITY:
		editorCommand(EvoGenerationCmd::Id::editSetBrushIntensity, msg.wParam);
		break;

	case EvoWorkThreadMessage::Id::DO_EDIT:
		editorCommand(EvoGenerationCmd::Id::editDoEdit, GridPoint24(Cast2UnsignedInt(msg.wParam), Cast2UnsignedInt(msg.lParam)));
		break;

	case EvoWorkThreadMessage::Id::SET_POI:
		editorCommand(EvoGenerationCmd::Id::editSetPOI, GridPoint24(Cast2UnsignedInt(msg.wParam), Cast2UnsignedInt(msg.lParam)));
		break;

	case EvoWorkThreadMessage::Id::SET_STRATEGY_COLOR:
		m_pColorManager->SetColor(static_cast<COLORREF>(msg.lParam), tColorObject::individual, static_cast<Strategy::Id>(msg.wParam));
		break;

	case EvoWorkThreadMessage::Id::SET_SELECTION_COLOR:
		m_pColorManager->SetColor(static_cast<COLORREF>(msg.lParam), tColorObject::selection);
		break;

	case EvoWorkThreadMessage::Id::SET_HIGHLIGHT_COLOR:
		m_pColorManager->SetColor(static_cast<COLORREF>(msg.lParam), tColorObject::highlight);
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}
