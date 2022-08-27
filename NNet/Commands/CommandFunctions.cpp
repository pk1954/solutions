// CommandFunctions.cpp
//
// Commands

module;

#include <cassert>
#include "NNetModelWriterInterface.h"
#include "NobType.h"
#include "Nob.h"

module CommandFunctions;

import DeleteBaseKnotCmd;
import DeletePipeCommand;
import DeleteIoConnectorCmd;
import NNetCommand;

using std::unique_ptr;

unique_ptr<NNetCommand> MakeDeleteCommand
(
	NNetModelWriterInterface const & nmwi,
	Nob                            & nob
)
{
	unique_ptr<NNetCommand> upCmd;
	if (nmwi.IsNobInModel(nob) && !nob.HasParentNob())
		switch (nob.GetNobType().GetValue())
		{
		using enum NobType::Value;
		case pipe:
			upCmd = make_unique<DeletePipeCommand>(nob);
			break;
		case inputConnector:
		case outputConnector: 
			upCmd = make_unique<DeleteIoConnectorCmd>(nob); 
			break;
		default:              
			upCmd = make_unique<DeleteBaseKnotCmd>(nob);  // May create IoLine
		}
	return move(upCmd);
}
