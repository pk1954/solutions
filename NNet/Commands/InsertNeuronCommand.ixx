// InsertNeuronCommand.ixx
//
// Commands

module;

#include <iostream>

export module InsertNeuronCommand;

import SplitPipeCommand;

export class InsertNeuronCommand : public SplitPipeCommand
{
public:
	InsertNeuronCommand
	(
		NobId         const  idPipe,
		MicroMeterPnt const& umSplitPoint
	)
		: SplitPipeCommand(idPipe)
	{
		m_upInsertNeuron = make_unique<Neuron>(umSplitPoint);
		SplitPipeCommand::InitSplit(*m_upInsertNeuron.get());
	}

	~InsertNeuronCommand() final = default;

	void Do() final
	{
		SplitPipeCommand::Do();
		m_pNMWI->Push2Model(move(m_upInsertNeuron));
	}

	void Undo() final
	{
		m_upInsertNeuron = m_pNMWI->PopFromModel<Neuron>();
		SplitPipeCommand::Undo();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push
	(
		NobId         const  idPipe,
		MicroMeterPnt const& umSplitPoint
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idPipe.GetValue() << umSplitPoint << endl;
		m_pStack->PushCommand(make_unique<InsertNeuronCommand>(idPipe, umSplitPoint));
	}

private:

	inline static const wstring NAME { L"InsertNeuron" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const idPipe       { ScrReadNobId(script) };
			MicroMeterPnt const umSplitPoint { ScrReadMicroMeterPnt(script) };
			InsertNeuronCommand::Push(idPipe, umSplitPoint);
		}
	};

	unique_ptr<Neuron> m_upInsertNeuron;
};
