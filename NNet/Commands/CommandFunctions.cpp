// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include <assert.h>
#include "NNetModelWriterInterface.h"
#include "DeletePipeCommand.h"
#include "DiscIoConnectorCmd.h"
#include "SplitNeuronCmd.h"
#include "DiscBaseKnotCmd.h"
#include "NNetCommand.h"
#include "NobType.h"
#include "IoConnector.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<NNetCommand> MakeDeleteCommand
(
	NNetModelWriterInterface & nmwi,
	Nob                      & nob
)
{
	unique_ptr<NNetCommand> upCmd;
	if (nmwi.IsNobInModel(nob))
		switch (nob.GetNobType().GetValue())
		{
		case NobType::Value::pipe:
			upCmd = make_unique<DeletePipeCommand>(nob);
			break;
		case NobType::Value::knot:
			break;
		case NobType::Value::inputConnector:
		case NobType::Value::outputConnector:
			upCmd = make_unique<DiscIoConnectorCmd>(nob, true);
			break;
		default:
			upCmd = make_unique<DiscBaseKnotCmd>(nob, true);
		}
	return move(upCmd);
}
