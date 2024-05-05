// NNetCommandHandler.ixx
//
// NNetWindows

module;

#include <Windows.h>

export module NNetWin32:NNetCommandHandler;

import Types;

export class NNetCommandHandler
{
public:
	virtual ~NNetCommandHandler() = default;
	virtual bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) = 0;
};				          
