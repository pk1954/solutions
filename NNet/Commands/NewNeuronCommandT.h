// NewNeuronCommandT.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"

template <typename T>
class NewNeuronCommandT : public Command
{
public:
	NewNeuronCommandT( MicroMeterPnt const & pos )
	{ 
		m_upNeuronT = make_unique<T>( pos );
	}

	~NewNeuronCommandT() {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Push2Model( move(m_upNeuronT) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upNeuronT = nmwi.PopFromModel<T>();
	}

private:
	unique_ptr<T> m_upNeuronT { nullptr };
};

using NewOutputNeuronCommand = NewNeuronCommandT<OutputNeuron>;
using NewInputNeuronCommand  = NewNeuronCommandT<InputNeuron>;
using NewNeuronCommand       = NewNeuronCommandT<Neuron>;
