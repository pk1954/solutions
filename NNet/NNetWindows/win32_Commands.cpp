// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NobIdList.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "NNetModelWriterInterface.h"
#include "Connect2PipeCommand.h"
#include "Connect2BaseKnotCommand.h"
#include "CommandStack.h"
#include "CommandFunctions.h"
#include "ConnAnimationCommand.h"
#include "PlugIoConnectorAnimation.h"
#include "PlugIoNeuronAnimation.h"
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

bool WinCommands::MakeIoConnector(RootWindow & win)
{
	if (IsTraceOn())
		TraceStream() << __func__ << endl;
	unique_ptr<ConnAnimationCommand> upCmd = { make_unique<ConnAnimationCommand>(win, *this) };
	bool bCmdOk = upCmd->IsCmdOk();
	if (bCmdOk)
		m_pCmdStack->PushCommand(move(upCmd));
	return bCmdOk;
}

void WinCommands::Connect(NobId const idSrc, NobId const idDst, RootWindow & win)
{
	if (IsTraceOn())
		TraceStream() << __func__ << L" " << idSrc << L" " << idDst << endl;

	unique_ptr<Command> upCmd;
	switch (m_pNMRI->GetNobType(idDst).GetValue())
	{
	case NobType::Value::pipe:
		upCmd = make_unique<Connect2PipeCommand>(idSrc, idDst);
		break;
	case NobType::Value::knot:
	case NobType::Value::neuron:
		upCmd = make_unique<Connect2BaseKnotCommand>(idSrc, idDst);
		break;
	case NobType::Value::inputNeuron:
		if (m_pNMRI->GetNobType(idSrc).IsKnotType())  // connect knot to output neuron
			upCmd = make_unique<Connect2BaseKnotCommand>(idSrc, idDst);
		else if (m_pNMRI->GetNobType(idSrc).IsOutputNeuronType())
			upCmd = make_unique<PlugIoNeuronAnimation>(* m_pNMWI, idSrc, idDst, win);
		else
			assert(false);
		break;
	case NobType::Value::outputNeuron:
		if (m_pNMRI->GetNobType(idSrc).IsKnotType())  // connect knot to output neuron
			upCmd = make_unique<Connect2BaseKnotCommand>(idSrc, idDst);
		else if (m_pNMRI->GetNobType(idSrc).IsOutputNeuronType())  // connect two output neurons
			upCmd = make_unique<Connect2BaseKnotCommand>(idSrc, idDst);
		else if (m_pNMRI->GetNobType(idSrc).IsInputNeuronType())
			upCmd = make_unique<PlugIoNeuronAnimation>(* m_pNMWI, idSrc, idDst, win);
		else
			assert(false);
		break;
	case NobType::Value::inputConnector:
	case NobType::Value::outputConnector:
		upCmd = make_unique<PlugIoConnectorAnimation>(* m_pNMWI, idSrc, idDst, win); 
		break;
	default:
		assert(false);
	}
	m_pCmdStack->PushCommand(move(upCmd));
}
