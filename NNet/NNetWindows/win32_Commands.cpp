// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "AlignDirectionCommand.h"
#include "AlignShapesCommand.h"
#include "PackShapesCommand.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"
#include "win32_Commands.h"

using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;

void WinCommands::Initialize
( 
	CommandStack      * const pCmdStack,
	NNetModelCommands * const pCmds
) 
{ 
	m_pCmdStack = pCmdStack;
	m_pCommands = pCmds;
}

void WinCommands::AlignDirection(RootWindow * const pWin)
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignDirectionCommand>(pWin, m_pCommands) );
}

void WinCommands::AlignShapes(RootWindow * const pWin)
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignShapesCommand>(pWin, m_pCommands) );
}

void WinCommands::PackShapes(RootWindow * const pWin)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<PackShapesCommand>(pWin, m_pCommands) );
}
