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
class EvoEditorWindow;
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
		DO_EDIT = WorkThreadMessage::FIRST_APP_MESSAGE,
		GOTO_ORIGIN,
		GOTO_DEATH,
		BENCHMARK,
		SET_BRUSH_RADIUS,
		SET_BRUSH_INTENSITY,
		SET_BRUSH_SHAPE,
		SET_BRUSH_OPERATOR,
		SET_BRUSH_MODE,
		SET_STRATEGY_COLOR,
		SET_SELECTION_COLOR,
		SET_HIGHLIGHT_COLOR,
		SET_POI,
		FIRST = DO_EDIT,
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
	GenerationCmd EvoCmd( EvoGenerationCmd::Id const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<GenerationCmd::Id>(cmd), param );  
	}  

	void editorCommand( EvoGenerationCmd::Id const cmd, WPARAM const wParam )
	{
		EditorCommand( static_cast<GenerationCmd::Id>(cmd), wParam );
	}

	void editorCommand( EvoGenerationCmd::Id const cmd, GridPoint24 const gp24 )
	{
		GetHistorySystem( )->CreateAppCommand( EvoCmd( cmd, gp24 ) );
	}

	virtual BOOL Dispatch( MSG const );

	ColorManager      * m_pColorManager;
    EvoHistorySysGlue * m_pEvoHistGlue;
};
