// CreateInitialNobsCmd.ixx
//
// NNetCommands

export module NNetCommands:CreateInitialNobsCmd;

import std;
import :NNetCommand;

using std::unique_ptr;
using std::make_unique;

export class CreateInitialNobsCmd : public NNetCommand
{
public:

	CreateInitialNobsCmd()
	{}

	void Do() final
	{
	};

	void Undo() final
	{
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pNMWI->CreateInitialNobs();
	}

private:

	inline static const wstring NAME { L"CreateInitialNobs" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			CreateInitialNobsCmd::Push();
		}
	} m_wrapper { NAME };
};
