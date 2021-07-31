// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include "DeletePipeCommand.h"
#include "DeleteClosedConnCmd.h"
#include "DiscIoConnectorCmd.h"
#include "SplitNeuronCmd.h"
#include "DiscBaseKnotCmd.h"
#include "NobType.h"
#include "ClosedConnector.h"
#include "IoConnector.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<Command> MakeDeleteCommand
(
	NNetModelWriterInterface & nmwi,
	Nob                      & nob
)
{
	unique_ptr<Command> upCmd;
	switch (nob.GetNobType().GetValue())
	{
	case NobType::Value::pipe:
		upCmd = make_unique<DeletePipeCommand>(nob);
		break;
	case NobType::Value::knot:
		break;
	case NobType::Value::inputConnector:
	case NobType::Value::outputConnector:
		upCmd = make_unique<DiscIoConnectorCmd>(nmwi, nob, true);
		break;
	case NobType::Value::closedConnector:
		upCmd = make_unique<DeleteClosedConnCmd>(nmwi, nob);
		break;
	default:
		upCmd = make_unique<DiscBaseKnotCmd>(nob, true);
	}
	return move(upCmd);
}
