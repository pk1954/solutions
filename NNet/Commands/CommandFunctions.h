// CommandFunctions.h
//
// Commands

#pragma once

#include "NobId.h"
#include "Command.h"

using std::unique_ptr;

class NNetModelWriterInterface;

unique_ptr<Command> MakeDeleteCommand(NNetModelWriterInterface &, Nob &);

//template <typename T>
//bool const CombineCmds(Command const & src) 
//{ 
//	if (typeid(src) != typeid(*this))
//		return false;
//	T const & srcCmd { static_cast<T const &>(src) };
//	if (GetAffectedNob() != srcCmd.GetAffectedNob())
//		return false;
//	m_radDelta += srcCmd.m_radDelta;
//	return true; 
//}
