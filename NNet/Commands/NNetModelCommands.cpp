// NNetModelCommands.cpp
//
// Commands

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>

module NNetModelCommands;

import AddNobsCommand;
import Commands;
import NNetCommands;
import NNetModel;
import NNetCommandStack;
import NNetModelCommands;
import NNetModelIO;
import Uniform2D;

using std::wstring;
using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;
using std::source_location;

void NNetModelCommands::Initialize
(
	NNetModelIO      * const pModelIO,
	Sound            * const pSound,
	NNetCommandStack * const pCmdStack
) 
{ 
	m_pModelIO  = pModelIO;
	m_pSound    = pSound;
	m_pCmdStack = pCmdStack;
}

void NNetModelCommands::SetModelInterface
(
	NNetModelWriterInterface * const pNMWI
) 
{ 
	m_pNMWI = pNMWI;
}

void NNetModelCommands::UndoCommand()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->UndoCommand())
		m_pSound->Warning();
}

void NNetModelCommands::RedoCommand()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->RedoCommand())
		m_pSound->Warning();
}

void NNetModelCommands::AddModel()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->MoveUPNobs()));
}

void NNetModelCommands::Connect(NobId const idSrc, NobId const idDst)
{ 
	using enum ConnectionType;

	ConnectionType connType { m_pNMWI->ConnectionResult(idSrc, idDst) };
	switch (connType)
	{
		case ct_fork: ConnectCreateForkCmd::Push(idSrc, idDst);  // case 1 
			break;
		case ct_synapse:	 
			ConnectCreateSynapseCmd::Push(idSrc, idDst);  // case 2
			break;
		case ct_neuron:
			Connect2NeuronCommand::Push(idSrc, idDst);    // case 3
			break;  
		case ct_knot:		 
			PlugIoLinesCmd::Push(idSrc, idDst);              // case 4/5
			break;
		case ct_connector:
			ConnAnimationCommand::Push(idSrc, idDst);     // case 12/13
			break;
		case ct_plugConnectors:
			PlugIoConnectorsCmd::Push(idSrc, idDst);         // case 6
			break;
		default: assert(false);
	}
	m_pSound->Play(L"SNAP_IN_SOUND");
}
