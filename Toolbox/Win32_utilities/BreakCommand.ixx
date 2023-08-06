// BreakCommand.ixx
//
// Win32_utilities

module;

#include <iostream>
#include <string>

export module BreakCommand;

import Script;
import Symtab;
import Wrapper;

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

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			int x = 42;
		}
	} m_wrapper { NAME };
};
