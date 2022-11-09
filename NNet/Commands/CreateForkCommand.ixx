// CreateForkCommand.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module CreateForkCommand;

import Types;
import Symtab;
import Script;
import Commands;
import NNetWrapperHelpers;
import NNetModel;
import NNetCommand;

using std::endl;
using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::pair;

export class CreateForkCommand : public NNetCommand
{
public:
	CreateForkCommand   // case 7 : Split Pipe, create fork and outgoing Pipe with OutputLine 
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
      : m_idPipe    (idPipe),
		m_pPipeOld  (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pStartNob(Cast2PosNob(m_pPipeOld->GetStartNobPtr())),
		m_pEndNob  (Cast2PosNob(m_pPipeOld->GetEndNobPtr()))
	{
		m_upFork       = make_unique<Fork>(pos);
		m_upOutputLine = make_unique<OutputLine>(pos + m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho  = make_unique<Pipe>(m_upFork.get(), m_upOutputLine.get());
		m_splitPipes   = m_pPipeOld->Split(*m_upFork.get());

		m_upFork->SetIncoming(m_splitPipes.first .get());
		m_upFork->SetOutgoing(m_splitPipes.second.get(), m_upPipeOrtho.get());

		m_upOutputLine->AddIncoming(*m_upPipeOrtho.get());
	}

	~CreateForkCommand() = default;

	void Do() final
	{
		m_pStartNob->ReplaceOutgoing(m_pPipeOld, m_splitPipes.first .get());
		m_pEndNob  ->ReplaceIncoming(m_pPipeOld, m_splitPipes.second.get());

		m_splitPipes.first ->FixSynapses();
		m_splitPipes.second->FixSynapses();

		m_pNMWI->Push2Model(move(m_upFork));
		m_pNMWI->Push2Model(move(m_splitPipes.first ));
		m_pNMWI->Push2Model(move(m_splitPipes.second));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upOutputLine));

		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_upPipeOld->FixSynapses();

		m_pNMWI->Restore2Model(move(m_upPipeOld));

		m_upOutputLine      = m_pNMWI->PopFromModel<OutputLine>();
		m_upPipeOrtho       = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_upFork            = m_pNMWI->PopFromModel<Fork>();

		m_pEndNob  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipeOld);
		m_pStartNob->ReplaceOutgoing(m_splitPipes.first .get(), m_pPipeOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << L" " << nobId << L" " << pos << endl;
		m_pStack->PushCommand(make_unique<CreateForkCommand>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"CreateFork" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			CreateForkCommand::Push(id, umPnt);
		}
	};

	NobId const m_idPipe;
	Pipe      * m_pPipeOld;
	PosNob    * m_pStartNob;  // start point of old Pipe
	PosNob    * m_pEndNob;    // end point of old Pipe

	pair<unique_ptr<Pipe>, unique_ptr<Pipe>> m_splitPipes;

	unique_ptr<Pipe>       m_upPipeOld;
	unique_ptr<Fork>       m_upFork;
	unique_ptr<OutputLine> m_upOutputLine;
	unique_ptr<Pipe>       m_upPipeOrtho;
};
