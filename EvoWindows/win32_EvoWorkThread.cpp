// win32_EvoWorkThread.cpp
//
// EvoWindows

#include "stdafx.h"
#include <sstream> 
#include "gridRect.h"
#include "SCRIPT.H"
#include "EvoHistorySysGlue.h"
#include "EventInterface.h"
#include "EvoReadBuffer.h"
#include "win32_delay.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_actionTimer.h"
#include "win32_colorManager.h"
#include "win32_util_resource.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_EvoWorkThread.h"

EvoWorkThread::EvoWorkThread
( 
	HWND                     const hwndApplication,
	ColorManager           * const pColorManager,
	ActionTimer            * const pActionTimer,
	EventInterface         * const pEvent,
	Delay                  * const pDelay,
	ObserverInterface      * const pObserver, 
	EvoHistorySysGlue      * const pEvoHistorySys,
	EvoWorkThreadInterface * const pWorkThreadInterface
) :
	WorkThread
	( 
		hwndApplication, 
		pActionTimer, 
		pEvent, 
		pDelay, 
		pObserver, 
		pEvoHistorySys->GetHistorySystem( ), 
		pWorkThreadInterface 
	),
	m_pColorManager( pColorManager ),
	m_pEvoHistGlue ( pEvoHistorySys )
{ }

EvoWorkThread::~EvoWorkThread( )
{
	m_pColorManager = nullptr;
	m_pEvoHistGlue  = nullptr;
}

BOOL EvoWorkThread::Dispatch( MSG const msg  )
{
	switch ( static_cast<EvoWorkThreadMessage::Id>(msg.message) )
	{

	case EvoWorkThreadMessage::Id::BENCHMARK:
		NGenerationSteps( static_cast<int>(msg.lParam) );
		break;

	case EvoWorkThreadMessage::Id::GOTO_ORIGIN:
	case EvoWorkThreadMessage::Id::GOTO_DEATH:
		GotoGeneration
		( 
			m_pEvoHistGlue->GetGenWithIndividual
			(
				GridPoint
				( 
					static_cast<GRID_COORD>( CastToShort(msg.wParam) ), 
					static_cast<GRID_COORD>( CastToShort(msg.lParam) )
				),
				static_cast<EvoWorkThreadMessage::Id>(msg.message) == EvoWorkThreadMessage::Id::GOTO_DEATH 
			)
		);
		break;

	case EvoWorkThreadMessage::Id::REDO:
		{
			HIST_GENERATION genCurrent  = GetHistorySystem( )->GetCurrentGeneration( );
			HIST_GENERATION genYoungest = GetHistorySystem( )->GetYoungestGeneration( );

			if ( ( genCurrent < genYoungest ) && isEditorCommand( genCurrent + 1 ) )
				GotoGeneration( genCurrent + 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case EvoWorkThreadMessage::Id::UNDO:
		{
			HIST_GENERATION genCurrent = GetHistorySystem( )->GetCurrentGeneration( );

			if ( ( genCurrent > 0 ) && isEditorCommand( genCurrent - 1 ) )
				GotoGeneration( genCurrent - 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_MODE:
		editorCommand( tEvoCmd::editSetBrushMode, msg.wParam );
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_RADIUS:
		editorCommand( tEvoCmd::editSetBrushRadius, msg.wParam );
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_SHAPE:
		editorCommand( tEvoCmd::editSetBrushShape, msg.wParam );
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_OPERATOR:
		editorCommand( tEvoCmd::editSetBrushManipulator, msg.wParam );
		break;

	case EvoWorkThreadMessage::Id::SET_BRUSH_INTENSITY:
		editorCommand( tEvoCmd::editSetBrushIntensity, msg.wParam );
		break;

	case EvoWorkThreadMessage::Id::DO_EDIT:
		editorCommand( tEvoCmd::editDoEdit, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
		break;

	case EvoWorkThreadMessage::Id::SET_POI:
		editorCommand( tEvoCmd::editSetPOI, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
		break;

	case EvoWorkThreadMessage::Id::SET_STRATEGY_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::individual, static_cast<Strategy::Id>(msg.wParam) );
		break;

	case EvoWorkThreadMessage::Id::SET_SELECTION_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::selection );
		break;

	case EvoWorkThreadMessage::Id::SET_HIGHLIGHT_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::highlight );
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}
