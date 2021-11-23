// NewNeuronCommandT.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"

template <typename T>
class NewNeuronCommandT : public NNetCommand
{
public:
	NewNeuronCommandT(MicroMeterPnt const & pos)
	{ 
		m_upNeuronT = make_unique<T>(pos);
	}

	~NewNeuronCommandT() {}

	void Do() final 
	{ 
		m_pNMWI->Push2Model(move(m_upNeuronT));
	}

	virtual void Undo()
	{ 
		m_upNeuronT = m_pNMWI->PopFromModel<T>();
	}

private:
	unique_ptr<T> m_upNeuronT { nullptr };
};

using NewOutputNeuronCommand = NewNeuronCommandT<OutputNeuron>;
using NewInputNeuronCommand  = NewNeuronCommandT<InputNeuron>;
