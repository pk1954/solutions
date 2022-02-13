// CommandFunctions.cpp
//
// Commands

#include "stdafx.h"
#include <assert.h>
#include "NNetModelWriterInterface.h"
#include "DeletePipeCommand.h"
#include "DiscIoConnectorCmd.h"
#include "SplitNeuronCmd.h"
#include "DeleteBaseKnotCmd.h"
#include "NNetCommand.h"
#include "NobType.h"
#include "IoConnector.h"
#include "BaseKnot.h"
#include "Nob.h"
#include "CommandFunctions.h"

unique_ptr<NNetCommand> MakeDeleteCommand
(
	NNetModelWriterInterface const & nmwi,
	Nob                            & nob
)
{
	unique_ptr<NNetCommand> upCmd;
	if (nmwi.IsNobInModel(nob))
		switch (nob.GetNobType().GetValue())
		{
		using enum NobType::Value;
		case pipe:            upCmd = make_unique<DeletePipeCommand>(nob);		  break;
		case inputConnector:
		case outputConnector: upCmd = make_unique<DiscIoConnectorCmd>(nob, true); break;
		default:              upCmd = make_unique<DeleteBaseKnotCmd>(nob);
		}
	return move(upCmd);
}
