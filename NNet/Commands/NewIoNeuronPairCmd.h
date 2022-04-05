// NewIoNeuronPairCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "IoNeuronPair.h"

class NewIoNeuronPairCmd : public NNetCommand
{
public:
	explicit NewIoNeuronPairCmd
	(
		NNetModelWriterInterface & nmwi,
		MicroMeterPnt      const & pos
	)
		: m_IoNeuronPair(nmwi, pos)
	{}

	~NewIoNeuronPairCmd() final = default;

	void Do() final 
	{
		m_IoNeuronPair.Push(*m_pNMWI);
	}

	void Undo() final
	{ 
		m_IoNeuronPair.Pop(*m_pNMWI);
	}

private:
	IoNeuronPair m_IoNeuronPair;
};
