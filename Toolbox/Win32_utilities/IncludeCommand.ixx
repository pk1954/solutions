// IncludeCommand.ixx
//
// Win32_utilities

export module IncludeCommand;

import std;
import Script;
import Symtab;
import ScriptFile;
import Commands;

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

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			wstring const& wstrFile = script.ScrReadString();
			if (!wstrFile.empty())
				StartScript(wstrFile, *m_pScriptHook);
		}
	} m_wrapper { NAME };

	inline static ScriptFunctor * m_pScriptHook { nullptr };
};
