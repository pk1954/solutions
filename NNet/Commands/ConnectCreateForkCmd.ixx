// ConnectCreateForkCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module ConnectCreateForkCmd;

import NNetModel;
import SplitPipeCommand;

using std::pair;

export class ConnectCreateForkCmd : public SplitPipeCommand
{
public:
	ConnectCreateForkCmd   // case 1 : Existing InputLine is connected to Pipe
	(
		NobId const idIoLine,
		NobId const idPipe
	)
	  : SplitPipeCommand(idPipe),
		m_idIoLine      (idIoLine),
		m_pInputLine    (m_pNMWI->GetNobPtr<InputLine*>(idIoLine))
	{
		m_upFork = make_unique<Fork>(m_pInputLine->GetPos());
		m_upFork->AddOutgoing(m_pInputLine->GetPipe());
		SplitPipeCommand::InitSplit(*m_upFork.get());
	}

	~ConnectCreateForkCmd() final = default;

	void Do() final
	{
		SplitPipeCommand::Do();
		m_pInputLine->GetPipe()->SetStartPnt(m_upFork.get());
		m_pInputLine->GetPipe()->PosChanged();
		m_pNMWI->Push2Model(move(m_upFork));
		m_upIoLine = m_pNMWI->RemoveFromModel<InputLine>(m_idIoLine);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upIoLine));
		m_upFork = m_pNMWI->PopFromModel<Fork>();
		m_pInputLine->GetPipe()->SetStartPnt(m_pInputLine);
		m_pInputLine->GetPipe()->PosChanged();
		SplitPipeCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId idSrc, NobId idDst)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idSrc.GetValue() << L' ' << idDst.GetValue() << endl;
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
};
