// DeleteNeuronInputCmd.ixx
//
// Commands
//
// handles only the input Pipes of a Neuron
// not the axon Pipe

module;

#include <memory>
#include <vector>

export module DeleteNeuronInputCmd;

import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::vector;

export class DeleteNeuronInputCmd : public NNetCommand
{
public:

	explicit DeleteNeuronInputCmd(NobId const id)
		: m_id(id)
	{
		Neuron* pNeuron { m_pNMWI->GetNobPtr<Neuron*>(m_id) };
		pNeuron->Apply2AllInPipes
		(
			[this](Pipe& pipe) { m_outputLines.push_back(make_unique<OutputLine>(pipe)); }
		);
	}

	void Do() final
	{
		m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_id);
		m_upNeuron->Apply2AllInPipes
		(
			[this](Pipe& pipe)
			{
				unique_ptr<OutputLine> upOutputLine { move(m_outputLines.back()) };
				pipe.SetEndPnt(upOutputLine.get());
				m_pNMWI->Push2Model(move(upOutputLine));
				m_outputLines.pop_back();
			}
		);
	}

	void Undo() final
	{
		m_upNeuron->Apply2AllInPipes
		(
			[this](Pipe& pipe)
			{
				m_outputLines.push_back(m_pNMWI->PopFromModel<OutputLine>());
				pipe.SetEndPnt(m_upNeuron.get());
			}
		);
		m_pNMWI->Restore2Model(move(m_upNeuron));
	}

private:
	NobId                          m_id;
	unique_ptr<Neuron>             m_upNeuron;
	vector<unique_ptr<OutputLine>> m_outputLines;
};
