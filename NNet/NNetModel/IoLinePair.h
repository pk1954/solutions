// IoLinePair.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "Pipe.h"

import MoreTypes;

using std::make_unique;

class IoLinePair
{
public:
	explicit IoLinePair
	(
		NNetModelWriterInterface & nmwi,
		MicroMeterPnt      const & pos
	)
	  :	m_upInputLine (make_unique<InputLine >(nmwi.StdSigGen(), pos - m_umOffset)),
	    m_upOutputLine(make_unique<OutputLine>(pos + m_umOffset)),
	    m_upPipe        (make_unique<Pipe>())
	{ 
		ConnectOutgoing(*m_upPipe.get(), *m_upInputLine.get());
		ConnectIncoming(*m_upPipe.get(), *m_upOutputLine.get());
	}

	void Push(NNetModelWriterInterface& nmwi)
	{
		nmwi.Push2Model(move(m_upInputLine));
		nmwi.Push2Model(move(m_upOutputLine));
		nmwi.Push2Model(move(m_upPipe));
	}

	void Pop(NNetModelWriterInterface& nmwi)
	{
		m_upPipe         = nmwi.PopFromModel<Pipe>();
		m_upOutputLine = nmwi.PopFromModel<OutputLine>();
		m_upInputLine  = nmwi.PopFromModel<InputLine>();
	}

private:
	inline static MicroMeterPnt const m_umOffset { 0.0_MicroMeter, 300.0_MicroMeter };

	unique_ptr<InputLine>  m_upInputLine  { };
	unique_ptr<OutputLine> m_upOutputLine { };
	unique_ptr<Pipe>       m_upPipe       { };
};