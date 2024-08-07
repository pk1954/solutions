// BreakCommand.ixx
//
// Toolbox\Commands

export module Commands:BreakCommand;

import std;
import RunTime;
import :Wrapper;

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
