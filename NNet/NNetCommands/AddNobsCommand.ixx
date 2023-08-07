// AddNobsCommand.ixx
//
// NNetCommands

module;

#include <memory>
#include <iostream>

export module NNetCommands:AddNobsCommand;

import :SelectionCommand;
import NNetModel;

using std::unique_ptr;

export class AddNobsCommand : public SelectionCommand
{
public:

	explicit AddNobsCommand(unique_ptr<UPNobList> upNobs2Add)
	  : m_upNobs2Add(move(upNobs2Add))
	{ 
		m_upNobs2Add->SelectAllNobs(true);
		m_upNobs2Add->CheckNobList();
	}

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->DeselectAllNobs();
		m_nrOfNobs = 0;
		while (m_upNobs2Add->IsNotEmpty())
		{
			if (UPNob upNob = m_upNobs2Add->Pop())
			{
				m_pNMWI->GetUPNobs().Push(move(upNob));
				++m_nrOfNobs;
			}
		}
		m_pNMWI->CheckModel();
	}

	void Undo() final
	{ 
		for (size_t i = 0; i < m_nrOfNobs; ++i)
			m_upNobs2Add->Push(m_pNMWI->GetUPNobs().Pop());
		SelectionCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		PushCommand(make_unique<AddNobsCommand>(move(CopySelectedNobs::Do(*m_pNMWI))));
	}

private:

	inline static const wstring NAME { L"AddNobs" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			AddNobsCommand::Push();
		}
	} m_wrapper { NAME };

	unique_ptr<UPNobList> m_upNobs2Add;
	size_t                m_nrOfNobs;
};
