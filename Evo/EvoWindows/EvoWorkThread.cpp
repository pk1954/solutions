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
import EvoWorkThreadInterface;
import EvoWorkThread:

EvoWorkThread::EvoWorkThread
(
	HWND                     const hwndApplication,
	ColorManager           * const pColorManager,
	EventInterface         * const pEvent,
	ObserverInterface      * const pObserver, 
	EvoHistorySysGlue      * const pEvoHistorySys,
	EvoWorkThreadInterface * const pWorkThreadInterface
) :
	WorkThread
	(
		hwndApplication, 
		pActionTimer, 
		pEvent, 
		pObserver, 
		pEvoHistorySys->GetHistorySystem(), 
		nullptr,
		pWorkThreadInterface,
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
