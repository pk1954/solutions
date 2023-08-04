// DeleteNeuronInputCmd.ixx
//
// Commands
//
// handles only the input Pipes of a Neuron and the Neuron itself
// not the axon Pipe

module;

#include <memory>
#include <vector>

export module NNetCommands:DeleteNeuronInputCmd;

import :NNetCommand;

using std::unique_ptr;
using std::make_unique;
using std::vector;

export class DeleteNeuronInputCmd : public NNetCommand
{
public:

	explicit DeleteNeuronInputCmd(Nob * const pNob)
		: m_pNeuron(Cast2Neuron(pNob))
	{
		m_pNeuron->Apply2AllInPipes
		(
			[this](Pipe& pipe) 
			{ 
				m_outputLines.push_back(make_unique<OutputLine>(pipe));
			}
		);
	}

	void Do() final
	{
		m_upNeuron = m_pNMWI->RemoveFromModel<Neuron>(m_pNeuron->GetId());
		m_upNeuron->Apply2AllInPipes
		(
			[this](Pipe& pipe)
			{
				unique_ptr<OutputLine> upOutputLine { move(m_outputLines.front()) };
				pipe.SetEndPnt(upOutputLine.get());
				m_pNMWI->Push2Model(move(upOutputLine));
				m_outputLines.erase(m_outputLines.begin());
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
	Neuron*                        m_pNeuron;
	unique_ptr<Neuron>             m_upNeuron;
	vector<unique_ptr<OutputLine>> m_outputLines;
};
