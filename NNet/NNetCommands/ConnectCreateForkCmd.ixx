// ConnectCreateForkCmd.ixx
//
// NNetCommands

export module NNetCommands:ConnectCreateForkCmd;

import std;
import IoUtil;
import NNetModel;
import :SplitPipeCommand;
import :NNetSound;

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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId idSrc, NobId idDst)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idSrc << idDst << endl;
		PushCommand(make_unique<ConnectCreateForkCmd>(idSrc, idDst));
		PlaySound(SNAP_IN_SOUND);
	}

private:

	inline static const wstring NAME { L"ConnectCreateFork" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId const idSrc { ScrReadNobId(script) };
			NobId const idDst { ScrReadNobId(script) };
			ConnectCreateForkCmd::Push(idSrc, idDst);
		}
	} m_wrapper { NAME };

	NobId           const m_idIoLine;
	InputLine     * const m_pInputLine;
	unique_ptr<Fork>      m_upFork;
	unique_ptr<InputLine> m_upIoLine;
};
