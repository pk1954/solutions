// CommandFunctions.h
//
// Commands

#pragma once

using std::unique_ptr;

class NNetModelWriterInterface;
class NNetCommand;
class Nob;

unique_ptr<NNetCommand> MakeDeleteCommand(NNetModelWriterInterface &, Nob &);
