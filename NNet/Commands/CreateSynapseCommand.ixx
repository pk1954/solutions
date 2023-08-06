// CreateSynapseCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:CreateSynapseCommand;

import Types;
import IoUtil;
import NNetModel;
import :SplitPipeCommand;

using std::unique_ptr;
using std::make_unique;

export class CreateSynapseCommand : public SplitPipeCommand
{
public:                    // pipe context menu: create synapse
	CreateSynapseCommand   // case 8 
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
      : SplitPipeCommand(idPipe)
	{
		OutputLine outputLine(pos);
		m_upInputLine = make_unique<InputLine>(pos - m_pNMWI->OrthoVector(idPipe));
		m_upSynapse   = make_unique<Synapse>(pos);
		m_upPipeAdd   = make_unique<Pipe>(m_upInputLine.get(), m_upSynapse.get());
		m_upSynapse  ->SetAddPipe(m_upPipeAdd.get());
		m_upInputLine->SetPipe(m_upPipeAdd.get());
		SplitPipeCommand::InitSplit(*m_upSynapse.get());
		m_upPipeAdd->PosChanged();
		m_upSynapse->Recalc();
	}

	~CreateSynapseCommand() = default;

	void Do() final
	{
		SplitPipeCommand::Do();
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_pNMWI->Push2Model(move(m_upPipeAdd));
		m_pNMWI->Push2Model(move(m_upSynapse));
	}

	void Undo() final
	{
		m_upSynapse   = m_pNMWI->PopFromModel<Synapse>();
		m_upPipeAdd   = m_pNMWI->PopFromModel<Pipe>();
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		SplitPipeCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << pos << endl;
		PushCommand(make_unique<CreateSynapseCommand>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"CreateSynapse" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			CreateSynapseCommand::Push(id, umPnt);
		}
	} m_wrapper { NAME };

	unique_ptr<InputLine> m_upInputLine;
	unique_ptr<Pipe>      m_upPipeAdd;
	unique_ptr<Synapse>   m_upSynapse;
};
