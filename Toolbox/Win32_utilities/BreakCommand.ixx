// BreakCommand.ixx
//
// Win32_utilities

module;

#include <iostream>
#include <string>

export module BreakCommand;

import Script;
import Symtab;
import WrapBase;

using std::wstring;

export class BreakCommand
{
public:

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

private:

	inline static const wstring NAME { L"Break" };

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			int x = 42;
		}
	} m_wrapper { NAME };
};
