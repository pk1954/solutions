// CommandFunctions.h
//
// Commands

#pragma once

using std::unique_ptr;

class Shape;
class BaseKnot;
class MainWindow;

unique_ptr<Command> MakeDeleteCommand    (Shape const &);
unique_ptr<Command> MakeDisconnectCommand(Shape const &);
