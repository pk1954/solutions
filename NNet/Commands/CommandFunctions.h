// CommandFunctions.h
//
// Commands

#pragma once

using std::unique_ptr;

class NNetModelWriterInterface;
class Command;
class Nob;

unique_ptr<Command> MakeDeleteCommand(NNetModelWriterInterface &, Nob &);
