// CommandFunctions.cpp
//
// Commands

module;

#include <cassert>
#include <memory>

module CommandFunctions;

import DeletePosNobCmd;
import DeletePipeCommand;
import DeleteIoConnectorCmd;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

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
			upCmd = make_unique<DeletePosNobCmd>(nob);  // May create IoLine
		}
	return move(upCmd);
}
