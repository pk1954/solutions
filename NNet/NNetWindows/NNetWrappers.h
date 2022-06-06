// NNetWrappers.h : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#pragma once

class NNetModelReaderInterface;
class NNetModelCommands;
class NNetModelIO;

extern void NNetWrappersSetModelInterface(NNetModelReaderInterface * const);

extern void InitializeNNetWrappers
(
    NNetModelCommands * const,
    NNetModelIO       * const
);
