// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
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
	CommandStack             * const pCmdStack,
	NNetModelCommands        * const pCmds,
	NNetModelWriterInterface * const pNMWI
) 
{ 
	m_pCmdStack = pCmdStack;
	m_pCommands = pCmds;
	m_pNMWI     = pNMWI;
}

void WinCommands::MakeConnector(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<ConnAnimationCommand>(win, *this) );
}

void WinCommands::ConnectConnector(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
//  TODO
}
