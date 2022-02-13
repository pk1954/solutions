// IoNeuronPair.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "Pipe.h"

using std::make_unique;

class IoNeuronPair
{
public:
	explicit IoNeuronPair(MicroMeterPnt const & pos)
	  :	m_upInputNeuron (make_unique<InputNeuron >(pos - m_umOffset)),
	    m_upOutputNeuron(make_unique<OutputNeuron>(pos + m_umOffset)),
	    m_upPipe        (make_unique<Pipe>())
	{ 
		ConnectOutgoing(*m_upPipe.get(), *m_upInputNeuron.get());
		ConnectIncoming(*m_upPipe.get(), *m_upOutputNeuron.get());
	}

	void Push(NNetModelWriterInterface& nmwi)
	{
		nmwi.Push2Model(move(m_upInputNeuron));
		nmwi.Push2Model(move(m_upOutputNeuron));
		nmwi.Push2Model(move(m_upPipe));
	}

	void Pop(NNetModelWriterInterface& nmwi)
	{
		m_upPipe         = nmwi.PopFromModel<Pipe>();
		m_upOutputNeuron = nmwi.PopFromModel<OutputNeuron>();
		m_upInputNeuron  = nmwi.PopFromModel<InputNeuron>();
	}

private:
	inline static MicroMeterPnt const m_umOffset { 0.0_MicroMeter, 300.0_MicroMeter };

	unique_ptr<InputNeuron>  m_upInputNeuron  { };
	unique_ptr<OutputNeuron> m_upOutputNeuron { };
	unique_ptr<Pipe>         m_upPipe         { };
};