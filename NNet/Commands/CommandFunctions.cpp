// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include "Connect2PipeCommand.h"
#include "Connect2BaseKnotCommand.h"
#include "DeletePipeCommand.h"
#include "DisconnectConnectorCommand.h"
#include "DisconnectBaseKnotCommand.h"
#include "ShapeType.h"
#include "BaseKnot.h"
#include "Shape.h"
#include "CommandFunctions.h"

unique_ptr<Command> MakeDeleteCommand(Shape const & shape)
{
	ShapeId const id { shape.GetId() };
	unique_ptr<Command> upCmd;
	if (shape.IsPipe()) 
		upCmd = make_unique<DeletePipeCommand>(id);
	else if (shape.IsConnector()) 
		upCmd = make_unique<DisconnectConnectorCommand>(id, true);
	else 
		upCmd = make_unique<DisconnectBaseKnotCommand>(id, true);
	return move(upCmd);
}

unique_ptr<Command> MakeDisconnectCommand(Shape const & shape)
{
	ShapeId const id { shape.GetId() };
	unique_ptr<Command> upCmd;
	if (shape.IsPipe()) 
		assert( false );
	else if (shape.IsConnector()) 
		upCmd = make_unique<DisconnectConnectorCommand>(id, false);
	else 
		upCmd = make_unique<DisconnectBaseKnotCommand>(id, false);
	return move(upCmd);
}

unique_ptr<Command> MakeConnectCommand(BaseKnot & baseKnotSrc, Shape & shapeDst)
{
	unique_ptr<Command> upCmd;
	if (shapeDst.IsPipe()) 
		upCmd = make_unique<Connect2PipeCommand>(&baseKnotSrc, static_cast<Pipe *>(&shapeDst) );
	else if (shapeDst.IsBaseKnot()) 
		upCmd = make_unique<Connect2BaseKnotCommand>(&baseKnotSrc, static_cast<BaseKnot *>(&shapeDst));
	else 
		assert( false );
	return move(upCmd);
}
