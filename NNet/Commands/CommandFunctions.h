// CommandFunctions.h
//
// Commands

#pragma once

using std::unique_ptr;

class Nob;
class BaseKnot;
class MainWindow;

unique_ptr<Command> MakeDeleteCommand    (Nob const &);
unique_ptr<Command> MakeDisconnectCommand(Nob const &);
