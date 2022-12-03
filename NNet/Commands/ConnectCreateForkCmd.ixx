// ConnectCreateForkCmd.ixx
//
// Commands

module;

#include <cassert>
#include <string>
#include <memory>
#include <iostream>

export module ConnectCreateForkCmd;

import Types;
import Symtab;
import Script;
import Commands;
import NNetWrapperHelpers;
import NNetCommand;
import NNetModel;

using std::endl;
using std::wstring;
using std::unique_ptr;
using std::make_unique;
using std::pair;

export class ConnectCreateForkCmd : public NNetCommand
{
public:
	ConnectCreateForkCmd   // case 1 : Existing InputLine is connected to Pipe
	(
		NobId const idIoLine,
		NobId const idPipe
	)
	  : m_idIoLine    (idIoLine),
		/////////////////
		m_idPipe2Split(idPipe),
		m_pPipe2Split (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pNobStart   (Cast2PosNob(m_pPipe2Split->GetStartNobPtr())),
		m_pNobEnd     (Cast2PosNob(m_pPipe2Split->GetEndNobPtr())),
		/////////////////
		m_pInputLine  (m_pNMWI->GetNobPtr<InputLine*>(idIoLine))
	{
		m_upFork = make_unique<Fork>(m_pInputLine->GetPos());
		m_upFork->AddOutgoing(m_pInputLine->GetPipe());

		/////////////////
		m_splitPipes  = m_pPipe2Split->Split(*m_upFork.get());
		m_fPosSplit   = m_pPipe2Split->PosOnPipe(m_pInputLine->GetPos());
		/////////////////
	}

	~ConnectCreateForkCmd() final = default;

	void Do() final
	{
		m_pInputLine->GetPipe()->SetStartPnt(m_upFork.get());

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
		m_upIoLine  = m_pNMWI->RemoveFromModel<InputLine>(m_idIoLine);
		m_upPipe2Split = m_pNMWI->RemoveFromModel<Pipe>     (m_idPipe2Split);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upIoLine));
		m_upFork = m_pNMWI->PopFromModel<Fork>();

		/////////////////
		m_pNMWI->Restore2Model(move(m_upPipe2Split));
		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_pNobEnd  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipe2Split);
		m_pNobStart->ReplaceOutgoing(m_splitPipes.first .get(), m_pPipe2Split);
		m_splitPipes.first ->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		m_splitPipes.second->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		/////////////////

		m_pInputLine->GetPipe()->SetStartPnt(m_pInputLine);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId idSrc, NobId idDst)
	{
		if (IsTraceOn())
			TraceStream() << NAME << L" " << idSrc << L" " << idDst << endl;
		m_pStack->PushCommand(make_unique<ConnectCreateForkCmd>(idSrc, idDst));
	}

private:

	inline static const wstring NAME { L"ConnectCreateFork" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId const idSrc { ScrReadNobId(script) };
			NobId const idDst { ScrReadNobId(script) };
			ConnectCreateForkCmd::Push(idSrc, idDst);
		}
	};

	NobId           const m_idIoLine;
	InputLine     * const m_pInputLine;
	unique_ptr<Fork>      m_upFork;
	unique_ptr<InputLine> m_upIoLine;

	NobId      const m_idPipe2Split;
	Pipe     * const m_pPipe2Split;
	PosNob   * const m_pNobStart;
	PosNob   * const m_pNobEnd;
	PipePair         m_splitPipes;
	unique_ptr<Pipe> m_upPipe2Split;
	float            m_fPosSplit;
};
