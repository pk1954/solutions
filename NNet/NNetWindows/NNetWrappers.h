// NNetWrappers.h : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#pragma once

class NNetModelReaderInterface;
class NNetModelCommands;
class NNetModelImporter;

extern void NNetWrappersSetModelInterface(NNetModelReaderInterface * const);

extern void InitializeNNetWrappers
(
    NNetModelCommands * const,
    NNetModelImporter * const
);
