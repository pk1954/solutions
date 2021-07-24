// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include "DeletePipeCommand.h"
#include "DeleteClosedConnCmd.h"
#include "DiscConnCmd.h"
#include "SplitNeuronCmd.h"
#include "DiscBaseKnotCmd.h"
#include "NobType.h"
#include "ClosedConnector.h"
#include "Connector.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<Command> MakeDeleteCommand(NNetModelWriterInterface & nmwi, Nob const & nob)
{
	return move(MakeDeleteCommand(nmwi, nob.GetId()));
}

unique_ptr<Command> MakeDeleteCommand
(
	NNetModelWriterInterface & nmwi,
	NobId                const id
)
{
	unique_ptr<Command> upCmd;
	Nob               * pNob { nmwi.GetNob(id) };
	assert(pNob);
	switch (pNob->GetNobType().GetValue())
	{
	case NobType::Value::pipe:
		upCmd = make_unique<DeletePipeCommand>(* pNob);
		break;
	case NobType::Value::knot:
		break;
	case NobType::Value::connector:
		upCmd = make_unique<DiscConnCmd>(nmwi, * pNob, true);
		break;
	case NobType::Value::closedConnector:
		upCmd = make_unique<DeleteClosedConnCmd>(nmwi, * pNob);
		break;
	default:
		upCmd = make_unique<DiscBaseKnotCmd>(* pNob, true);
	}
	return move(upCmd);
}

unique_ptr<Command> MakeDisconnectCommand
(
	NNetModelWriterInterface & nmwi,
	NobId                const id
)
{
	Nob * pNob { nmwi.GetNob(id) };
	if ( ! pNob )
		return nullptr;

	unique_ptr<Command> upCmd;
	switch ( pNob->GetNobType().GetValue() )
	{
	case NobType::Value::pipe:
		assert( false );
		break;
	case NobType::Value::connector:
		upCmd = make_unique<DiscConnCmd>(nmwi, *pNob, false);
		break;
	case NobType::Value::closedConnector:
		assert(false);
		break;
	default:
		upCmd = make_unique<DiscBaseKnotCmd>(*pNob, false);
	}
	return move(upCmd);
}
