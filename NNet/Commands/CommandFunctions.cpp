// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include "DeletePipeCommand.h"
#include "DisconnectConnectorCommand.h"
#include "DisconnectBaseKnotCommand.h"
#include "NobType.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<Command> MakeDeleteCommand(Nob const & nob)
{
	NobId const id { nob.GetId() };
	unique_ptr<Command> upCmd;
	if (nob.IsPipe()) 
		upCmd = make_unique<DeletePipeCommand>(id);
	else if (nob.IsConnector()) 
		upCmd = make_unique<DisconnectConnectorCommand>(id, true);
	else 
		upCmd = make_unique<DisconnectBaseKnotCommand>(id, true);
	return move(upCmd);
}

unique_ptr<Command> MakeDisconnectCommand(Nob const & nob)
{
	NobId const id { nob.GetId() };
	unique_ptr<Command> upCmd;
	if (nob.IsPipe()) 
		assert( false );
	else if (nob.IsConnector()) 
		upCmd = make_unique<DisconnectConnectorCommand>(id, false);
	else 
		upCmd = make_unique<DisconnectBaseKnotCommand>(id, false);
	return move(upCmd);
}
