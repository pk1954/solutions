// IncludeCommand.ixx
//
// Win32_utilities

module;

#include <iostream>
#include <string>

export module IncludeCommand;

import Script;
import Symtab;
import Commands;
import WrapBase;

using std::wstring;

export class IncludeCommand
{
public:

	static void Register(ScriptFunctor& scriptHook)
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
		m_pScriptHook = &scriptHook;
	}

private:

	inline static const wstring NAME { L"Include" };

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			wstring const& wstrFile = script.ScrReadString();
			if (!wstrFile.empty())
				::StartScript(wstrFile, *m_pScriptHook);
		}
	} m_wrapper { NAME };

	inline static ScriptFunctor * m_pScriptHook { nullptr };
};
