// CommandFunctions.h
//
// Commands

#pragma once

using std::unique_ptr;

class Shape;
class BaseKnot;

unique_ptr<Command> MakeDeleteCommand    (Shape const &);
unique_ptr<Command> MakeDisconnectCommand(Shape const &);
unique_ptr<Command> MakeConnectCommand   (BaseKnot &, Shape &);
