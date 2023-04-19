// NNetWrappers.ixx : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

module;

#include <compare>

export module NNetWin32:NNetWrappers;

import NNetModelIO;
import Script;

export void InitializeNNetWrappers(NNetModelIO * const, ScriptFunctor* const);
