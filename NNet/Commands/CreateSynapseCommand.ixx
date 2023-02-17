// CreateSynapseCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module CreateSynapseCommand;

import Types;
import NNetModel;
import NNetCommand;

using std::unique_ptr;
using std::make_unique;

export class CreateSynapseCommand : public NNetCommand
{
public:                    // pipe context menu: create synapse
	CreateSynapseCommand   // case 8 
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
      : m_idPipe(idPipe),
		m_pPipe(m_pNMWI->GetNobPtr<Pipe*>(idPipe))
	{
		OutputLine outputLine(pos);
		m_upInputLine = make_unique<InputLine>(pos - m_pNMWI->OrthoVector(idPipe));
		m_upSynapse   = make_unique<Synapse>(pos);
		m_upPipeAdd   = make_unique<Pipe>(m_upInputLine.get(), m_upSynapse.get());
		m_upPipeIn    = make_unique<Pipe>(m_pPipe->GetStartNobPtr(), m_upSynapse.get());
		m_upPipeOut   = make_unique<Pipe>(m_upSynapse.get(),         m_pPipe->GetEndNobPtr());
		m_upSynapse->SetAddPipe(m_upPipeAdd.get());
		m_upSynapse->AddIncoming(m_upPipeIn.get());
		m_upSynapse->AddOutgoing(m_upPipeOut.get());
		m_upInputLine->SetPipe(m_upPipeAdd.get());
		m_upPipeAdd->PosChanged();
	}

	~CreateSynapseCommand() = default;

	void Do() final
	{
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_pNMWI->Push2Model(move(m_upPipeAdd));
		m_pNMWI->Push2Model(move(m_upSynapse));
		m_pNMWI->Push2Model(move(m_upPipeIn));
		m_pNMWI->Push2Model(move(m_upPipeOut));
		m_upPipe = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipe));
		m_upPipeOut   = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeIn    = m_pNMWI->PopFromModel<Pipe>();
		m_upSynapse   = m_pNMWI->PopFromModel<Synapse>();
		m_upPipeAdd   = m_pNMWI->PopFromModel<Pipe>();
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << pos << endl;
		m_pStack->PushCommand(make_unique<CreateSynapseCommand>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"CreateSynapse" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			CreateSynapseCommand::Push(id, umPnt);
		}
	};

	NobId  const          m_idPipe;
	Pipe * const          m_pPipe;
	unique_ptr<Pipe>      m_upPipe;
	unique_ptr<Pipe>      m_upPipeIn;
	unique_ptr<Pipe>      m_upPipeOut;
	unique_ptr<InputLine> m_upInputLine;
	unique_ptr<Pipe>      m_upPipeAdd;
	unique_ptr<Synapse>   m_upSynapse;
};
