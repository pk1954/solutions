// win32_NNetWorkThread.h
//
// NNetWindows

#pragma once

#include "HistoryGeneration.h"
#include "win32_WorkThread.h"

class Delay;
class ActionTimer;
class RootWindow;
class WinManager;
class EventInterface;
class NNetHistorySysGlue;
class NNetWorkThreadInterface;

class NNetWorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REDO = WorkThreadMessage::FIRST_APP_MESSAGE,
		UNDO,
		RESET_MODEL,
		FIRST = REDO,
		LAST = RESET_MODEL
	};

	static BOOL IsValid( NNetWorkThreadMessage::Id msg )
	{
		return (NNetWorkThreadMessage::Id::FIRST <= msg) && (msg <= NNetWorkThreadMessage::Id::LAST);
	}
};

class NNetWorkThread: public WorkThread
{
public:
	NNetWorkThread
	( 
		HWND                      const,
		ActionTimer             * const,
		EventInterface          * const,
		Delay                   * const,
		ObserverInterface       * const,
		NNetHistorySysGlue      * const,
		NNetWorkThreadInterface * const
	);
	~NNetWorkThread( );

private:

	bool isEditorCommand( HIST_GENERATION const gen )
	{
		return ::IsEditorCommand( static_cast<tNNetCmd>( GetHistorySystem( )->GetGenerationCmd( gen ) ) );
	}

	GenerationCmd NNetCmd( tNNetCmd const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

	virtual BOOL Dispatch( MSG const );
};
