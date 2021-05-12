// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include "DeletePipeCommand.h"
#include "DiscClosedConnCmd.h"
#include "DiscConnCmd.h"
#include "DiscBaseKnotCmd.h"
#include "NobType.h"
#include "ClosedConnector.h"
#include "Connector.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<Command> MakeDeleteCommand
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
		upCmd = make_unique<DeletePipeCommand>(nmwi, id);
		break;
	case NobType::Value::knot:
		break;
	case NobType::Value::connector:
		upCmd = make_unique<DiscConnCmd>(nmwi, id, true);
		break;
	case NobType::Value::closedConnector:
		upCmd = make_unique<DiscClosedConnCmd>(nmwi, id, true);
		break;
	default:
		upCmd = make_unique<DiscBaseKnotCmd>(nmwi, id, true);
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
		upCmd = make_unique<DiscConnCmd>(nmwi, id, false);
		break;
	case NobType::Value::closedConnector:
		upCmd = make_unique<DiscClosedConnCmd>(nmwi, id, false);
		break;
	default:
		upCmd = make_unique<DiscBaseKnotCmd>(nmwi, id, false);
	}
	return move(upCmd);
}
