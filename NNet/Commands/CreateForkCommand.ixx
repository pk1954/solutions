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
		MicroMeterPnt const& umSplitPoint
	)
      : 
		/////////////////
		m_idPipe2Split(idPipe),
		m_pPipe2Split (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pNobStart   (Cast2PosNob(m_pPipe2Split->GetStartNobPtr())),
		m_pNobEnd     (Cast2PosNob(m_pPipe2Split->GetEndNobPtr()))
		/////////////////
	{
		m_upFork       = make_unique<Fork>(umSplitPoint);
		m_upOutputLine = make_unique<OutputLine>(umSplitPoint + m_pNMWI->OrthoVector(m_idPipe2Split));
		m_upPipeOrtho  = make_unique<Pipe>(m_upFork.get(), m_upOutputLine.get());
		
		/////////////////
		m_splitPipes   = m_pPipe2Split->Split(*m_upFork.get());
		m_fPosSplit    = m_pPipe2Split->PosOnPipe(m_upFork->GetPos());
		/////////////////

		m_upFork      ->AddOutgoing(m_upPipeOrtho.get());
		m_upOutputLine->AddIncoming(m_upPipeOrtho.get());
	}

	~CreateForkCommand() = default;

	void Do() final
	{
		/////////////////
		m_pNobStart->ReplaceOutgoing(m_pPipe2Split, m_splitPipes.first .get());
		m_pNobEnd  ->ReplaceIncoming(m_pPipe2Split, m_splitPipes.second.get());
		m_pPipe2Split->Apply2AllSynapses
		(
			[this](Nob* pNob)
			{
				Pipe* pPipeNew { SelectPipe(pNob, m_splitPipes, m_fPosSplit) };
				Cast2Synapse(pNob)->SetMainPipe(pPipeNew);
			}
		);
		m_pNMWI->Push2Model(move(m_splitPipes.first ));
		m_pNMWI->Push2Model(move(m_splitPipes.second));
		/////////////////

		m_pNMWI->Push2Model(move(m_upFork));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upOutputLine));

		m_upPipe2Split = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe2Split);
	}

	void Undo() final
	{
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upPipeOrtho  = m_pNMWI->PopFromModel<Pipe>();
		m_upFork       = m_pNMWI->PopFromModel<Fork>();

		/////////////////
		m_pNMWI->Restore2Model(move(m_upPipe2Split));
		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_pNobEnd  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipe2Split);
		m_pNobStart->ReplaceOutgoing(m_splitPipes.first .get(), m_pPipe2Split);
		m_splitPipes.first ->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		m_splitPipes.second->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		/////////////////
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

	unique_ptr<Fork>       m_upFork;
	unique_ptr<OutputLine> m_upOutputLine;
	unique_ptr<Pipe>       m_upPipeOrtho;

	NobId      const m_idPipe2Split;
	Pipe     * const m_pPipe2Split;
	PosNob   * const m_pNobStart;
	PosNob   * const m_pNobEnd;
	PipePair         m_splitPipes;
	unique_ptr<Pipe> m_upPipe2Split;
	float            m_fPosSplit;
};
