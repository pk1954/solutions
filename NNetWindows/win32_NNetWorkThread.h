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
		FIRST = REDO,
		LAST = UNDO
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

	GenerationCmd NNetCmd( tNNetCmd const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

	//void editorCommand( tGenCmd const cmd, WPARAM const wParam )
	//{
	//	GetHistorySystem( )->CreateAppCommand( NNetCmd( static_cast<tNNetCmd>(cmd), Int24(CastToUnsignedInt(wParam)) ) );
	//}

	bool isEditorCommand( HIST_GENERATION const gen )
	{
		return ::IsEditorCommand( static_cast<tNNetCmd>( GetHistorySystem( )->GetGenerationCmd( gen ) ) );
	}

	virtual BOOL Dispatch( MSG const );
};
