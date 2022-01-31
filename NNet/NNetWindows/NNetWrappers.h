// NNetWrappers.h : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#pragma once

class NNetModelReaderInterface;
class NNetModelCommands;
class NNetModelImporter;

extern void DefineNNetWrappers
(
    NNetModelReaderInterface * const, 
    NNetModelCommands        * const,
    NNetModelImporter        * const
);
