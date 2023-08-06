// CreateForkCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:CreateForkCommand;

import IoUtil;
import NNetModel;
import :SplitPipeCommand;

using std::pair;

export class CreateForkCommand : public SplitPipeCommand
{
public:
	CreateForkCommand   // case 7 : Split Pipe, create fork and outgoing Pipe with OutputLine 
	(
		NobId         const  idPipe,
		MicroMeterPnt const& umSplitPoint
	)
      : SplitPipeCommand(idPipe)
	{
		m_upFork       = make_unique<Fork>(umSplitPoint);
		m_upOutputLine = make_unique<OutputLine>(umSplitPoint + m_pNMWI->OrthoVector(idPipe));
		m_upPipeOrtho  = make_unique<Pipe>(m_upFork.get(), m_upOutputLine.get());
		m_upFork      ->AddOutgoing(m_upPipeOrtho.get());
		m_upOutputLine->AddIncoming(m_upPipeOrtho.get());
		SplitPipeCommand::InitSplit(*m_upFork.get());
	}

	~CreateForkCommand() = default;

	void Do() final
	{
		SplitPipeCommand::Do();
		m_pNMWI->Push2Model(move(m_upFork));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upOutputLine));
	}

	void Undo() final
	{
		m_upOutputLine = m_pNMWI->PopFromModel<OutputLine>();
		m_upPipeOrtho  = m_pNMWI->PopFromModel<Pipe>();
		m_upFork       = m_pNMWI->PopFromModel<Fork>();
		SplitPipeCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << pos << endl;
		PushCommand(make_unique<CreateForkCommand>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"CreateFork" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			CreateForkCommand::Push(id, umPnt);
		}
	} m_wrapper { NAME };

	unique_ptr<Fork>       m_upFork;
	unique_ptr<OutputLine> m_upOutputLine;
	unique_ptr<Pipe>       m_upPipeOrtho;
};
