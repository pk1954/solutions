// win32_workThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "win32_WorkThread.h"

class WorkThreadInterface
{
public:
	WorkThreadInterface( );
    ~WorkThreadInterface( );

	void Initialize( std::wostream * );

	void Start( WorkThread * const );
	void Stop( );

	void PostUndo( );
	void PostRedo( );
	void PostReset( BOOL );
	void PostRunGenerations( BOOL const );
	void PostStopComputation();
	void PostPrevGeneration();
	void PostGenerationStep();
	void PostRepeatGenerationStep();       // Do not call! Used by WorkThread only;


	BOOL IsRunning    ( ) const	{ return m_pWorkThread->IsRunning    ( ); }
	BOOL IsAsyncThread( ) const	{ return m_pWorkThread->IsAsyncThread( ); }

protected:

	BOOL            IsTraceOn  ( ) const { return   m_bTrace; }
	std::wostream & TraceStream( )       { return * m_pTraceStream; }

	void WorkMessage
	( 
		BOOL                  const isEditOperation,
		WorkThreadMessage::Id const msg,
		WPARAM                const wparam, 
		LPARAM                const lparam
	)
	{
		m_pWorkThread->WorkMessage( isEditOperation, msg, wparam, lparam );
	}

	void Continue( ) { m_pWorkThread->Continue(); }

private:
	WorkThread     * m_pWorkThread;
    std::wostream  * m_pTraceStream;
	BOOL             m_bTrace;
}; 
