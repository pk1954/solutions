// NNetCommandHandler.ixx
//
// NNetWindows

export module NNetWin32:NNetCommandHandler;

import Types;
import WinBasics;

export class NNetCommandHandler
{
public:
	virtual ~NNetCommandHandler() = default;
	virtual bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) = 0;
};				          
