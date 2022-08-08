// NewIoLinePairCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "IoLinePair.h"

import MoreTypes;

class NewIoLinePairCmd : public NNetCommand
{
public:
	explicit NewIoLinePairCmd
	(
		NNetModelWriterInterface & nmwi,
		MicroMeterPnt      const & pos
	)
		: m_IoLinePair(nmwi, pos)
	{}

	~NewIoLinePairCmd() final = default;

	void Do() final 
	{
		m_IoLinePair.Push(*m_pNMWI);
	}

	void Undo() final
	{ 
		m_IoLinePair.Pop(*m_pNMWI);
	}

private:
	IoLinePair m_IoLinePair;
};
