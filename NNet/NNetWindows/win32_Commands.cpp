// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "AlignDirectionCommand.h"
#include "AlignPositionsCommand.h"
#include "PackShapesCommand.h"
#include "CreateConnectorCommand.h"
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

void WinCommands::Update(ConnAnimationCommand * const pCAC)
{
	if (pCAC)
		m_pNMWI->SetConnNeurons(pCAC->GetActual(), pCAC->GetAnimatedShapes());
}

void WinCommands::AlignShapes(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignPositionsCommand>(win, *this, 0, false) );
}

void WinCommands::MakeConnector(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignPositionsCommand>(win, *this, 1, false) );
}

void WinCommands::ConnectConnector(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
//  TODO
}
