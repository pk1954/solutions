// ConnectCreateSynapseCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:ConnectCreateSynapseCmd;

import NNetModel;
import :SplitPipeCommand;

using std::unique_ptr;
using std::make_unique;

export class ConnectCreateSynapseCmd : public SplitPipeCommand
{
public:
	ConnectCreateSynapseCmd   // case 2: OutputLine connects to Pipe
	(
		NobId const idOutputLine,
		NobId const idPipe
	)
      : SplitPipeCommand(idPipe), 
		m_idOutputLine(idOutputLine),
		m_pOutputLine(m_pNMWI->GetNobPtr<OutputLine*>(idOutputLine))
	{
		m_upSynapse = make_unique<Synapse>(m_pOutputLine->GetPos());
		m_upSynapse->SetAddPipe(m_pOutputLine->GetPipe());
		SplitPipeCommand::InitSplit(*m_upSynapse.get());
		m_upSynapse->Recalc();
	}

	~ConnectCreateSynapseCmd() final = default;

	void Do() final
	{
		SplitPipeCommand::Do();
		m_pOutputLine->GetPipe()->SetEndPnt(m_upSynapse.get());
		m_pOutputLine->GetPipe()->PosChanged();
		m_pNMWI->Push2Model(move(m_upSynapse));
		m_upOutputLine = m_pNMWI->RemoveFromModel<OutputLine>(m_idOutputLine);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upOutputLine));
		m_upSynapse = m_pNMWI->PopFromModel<Synapse>();
		m_pOutputLine->GetPipe()->SetEndPnt(m_pOutputLine);
		m_pOutputLine->GetPipe()->PosChanged();
		SplitPipeCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId idSrc, NobId idDst)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idSrc.GetValue() << L' ' << idDst.GetValue() << endl;
		PushCommand(make_unique<ConnectCreateSynapseCmd>(idSrc, idDst));
		m_pSound->Play(L"SNAP_IN_SOUND");
	}

private:

	inline static const wstring NAME { L"ConnectCreateSynapse" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			NobId const idSrc { ScrReadNobId(script) };
			NobId const idDst { ScrReadNobId(script) };
			ConnectCreateSynapseCmd::Push(idSrc, idDst);
		}
	} m_wrapper;

	NobId       const m_idOutputLine;
	OutputLine* const m_pOutputLine;

	unique_ptr<Synapse>    m_upSynapse;
	unique_ptr<OutputLine> m_upOutputLine;
};
