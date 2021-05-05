// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "Connect2PipeCommand.h"
#include "Connect2BaseKnotCommand.h"
#include "CommandStack.h"
#include "CommandFunctions.h"
#include "ConnAnimationCommand.h"
#include "PluginAnimationCommand.h"
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
	NNetModelReaderInterface * const pNMRI,
	NNetModelWriterInterface * const pNMWI
) 
{ 
	m_pCmdStack = pCmdStack;
	m_pCommands = pCmds;
	m_pNMRI     = pNMRI;
	m_pNMWI     = pNMWI;
}

void WinCommands::MakeConnector(RootWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<ConnAnimationCommand>(win, *this) );
}

void WinCommands::Connect(ShapeId const idSrc, ShapeId const idDst, MainWindow & win)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << idSrc << L" " << idDst << endl;

	unique_ptr<Command> upCmd;
	switch ( m_pNMRI->GetShapeType(idDst).GetValue() )
	{
	case ShapeType::Value::pipe:
		upCmd = make_unique<Connect2PipeCommand>    
		(
			m_pNMWI->GetShapePtr<BaseKnot *>(idSrc), 
			m_pNMWI->GetShapePtr<Pipe     *>(idDst)
		);
		break;
	case ShapeType::Value::knot:
	case ShapeType::Value::neuron:
	case ShapeType::Value::inputNeuron:
	case ShapeType::Value::outputNeuron:
		upCmd = make_unique<Connect2BaseKnotCommand>
		(
			m_pNMWI->GetShapePtr<BaseKnot *>(idSrc), 
			m_pNMWI->GetShapePtr<BaseKnot *>(idDst)
		);
	break;
	case ShapeType::Value::connector:
		upCmd = make_unique<PluginAnimationCommand> 
		(
			* m_pNMWI->GetShapePtr<Connector *>(idSrc), 
			* m_pNMWI->GetShapePtr<Connector *>(idDst),
			win,
			* this
		);
		break;
	default:
		assert( false );
	}
	m_pCmdStack->PushCommand(move(upCmd));
}
