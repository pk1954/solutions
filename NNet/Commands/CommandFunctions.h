// CommandFunctions.h
//
// Commands

#pragma once

#include "NobId.h"

using std::unique_ptr;

class NNetModelWriterInterface;

unique_ptr<Command> MakeDeleteCommand(NNetModelWriterInterface &, Nob &);
