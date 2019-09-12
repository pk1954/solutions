// win32_EvoWorkThread.h
//
// EvoWindows

#pragma once

#include "GridPoint24.h"
#include "HistoryGeneration.h"
#include "win32_WorkThread.h"

class Delay;
class ActionTimer;
class ColorManager;
class EditorWindow;
class RootWindow;
class EvolutionCore;
class WinManager;
class HistorySystem;
class EventInterface;
class EvoHistorySysGlue;
class EvoWorkThreadInterface;

class EvoWorkThreadMessage
{
public:
	enum class Id : UINT
	{
	    REDO = WorkThreadMessage::FIRST_APP_MESSAGE,
		UNDO,
		GOTO_ORIGIN,
		GOTO_DEATH,
		BENCHMARK,
		DO_EDIT,
		SET_BRUSH_RADIUS,
		SET_BRUSH_INTENSITY,
		SET_BRUSH_SHAPE,
		SET_BRUSH_OPERATOR,
		SET_BRUSH_MODE,
		SET_STRATEGY_COLOR,
		SET_SELECTION_COLOR,
		SET_HIGHLIGHT_COLOR,
		SET_POI,
		FIRST = REDO,
		LAST = SET_POI
	};

	static BOOL IsValid( EvoWorkThreadMessage::Id msg )
	{
		return (EvoWorkThreadMessage::Id::FIRST <= msg) && (msg <= EvoWorkThreadMessage::Id::LAST);
	}
};

class EvoWorkThread: public WorkThread
{
public:
	EvoWorkThread
	( 
		HWND                     const,
		ColorManager           * const,
		ActionTimer            * const,
		EventInterface         * const,
		Delay                  * const,
		ObserverInterface      * const,
		EvoHistorySysGlue      * const,
		EvoWorkThreadInterface * const
	);
	~EvoWorkThread( );
	
private:
	GenerationCmd EvoCmd( tEvoCmd const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

	void editorCommand( tEvoCmd const cmd, WPARAM const wParam )
	{
		EditorCommand( static_cast<tGenCmd>(cmd), wParam );
	}

	void editorCommand( tEvoCmd const cmd, GridPoint24 const gp24 )
	{
		GetHistorySystem( )->CreateAppCommand( EvoCmd( cmd, gp24 ) );
	}

	bool isEditorCommand( HIST_GENERATION const gen )
	{
		return ::IsEditorCommand( static_cast<tEvoCmd>( GetHistorySystem( )->GetGenerationCmd( gen ) ) );
	}

	virtual BOOL Dispatch( MSG const );

	ColorManager      * m_pColorManager;
    EvoHistorySysGlue * m_pEvoHistGlue;
};
