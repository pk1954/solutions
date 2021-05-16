// CommandFunctions.h
//
// Commands

#pragma once

#include "NobId.h"

using std::unique_ptr;

class NNetModelWriterInterface;

unique_ptr<Command> MakeDeleteCommand    (NNetModelWriterInterface &, Nob const &);

unique_ptr<Command> MakeDeleteCommand    (NNetModelWriterInterface &, NobId const);
unique_ptr<Command> MakeDisconnectCommand(NNetModelWriterInterface &, NobId const);
