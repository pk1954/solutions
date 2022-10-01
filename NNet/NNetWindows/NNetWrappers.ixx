// NNetWrappers.ixx : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

module;

#include <compare>

export module NNetWrappers;

import NNetModelCommands;
import NNetModelIO;
import NNetModel;

export void NNetWrappersSetModelInterface(NNetModelReaderInterface * const);

export void InitializeNNetWrappers
(
    NNetModelCommands * const,
    NNetModelIO       * const
);
