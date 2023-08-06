// ResetModelCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:ResetModelCmd;

import Types;
import :NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class ResetModelCmd : public NNetCommand
{
public:

	ResetModelCmd()
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
		m_pNMWI->ResetModel();
		ClearStack();
	}

private:

	inline static const wstring NAME { L"ResetModel" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			ResetModelCmd::Push();
		}
	} m_wrapper { NAME };
};
