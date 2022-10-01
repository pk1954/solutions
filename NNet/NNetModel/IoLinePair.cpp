// IoLinePair.cpp
//
// Commands

module;

#include <memory>

module NNetModel:IoLinePair;

import Types;
import :NNetModelWriterInterface;
import :Pipe;
import :BaseKnot;
import :InputLine;
import :OutputLine;

using std::unique_ptr;
using std::make_unique;

IoLinePair::IoLinePair
(
	NNetModelWriterInterface & nmwi,
	MicroMeterPnt      const & pos
)
  :	m_upInputLine (make_unique<InputLine >(nmwi.StdSigGen(), pos - m_umOffset)),
	m_upOutputLine(make_unique<OutputLine>(pos + m_umOffset)),
	m_upPipe      (make_unique<Pipe>())
{
	ConnectOutgoing(* m_upPipe.get(), * m_upInputLine.get());
	ConnectIncoming(* m_upPipe.get(), * m_upOutputLine.get());
}

IoLinePair::~IoLinePair() = default;

void IoLinePair::Push(NNetModelWriterInterface& nmwi)
{
	nmwi.Push2Model(move(m_upInputLine));
	nmwi.Push2Model(move(m_upOutputLine));
	nmwi.Push2Model(move(m_upPipe));
}

void IoLinePair::Pop(NNetModelWriterInterface& nmwi)
{
	m_upPipe       = nmwi.PopFromModel<Pipe>();
	m_upOutputLine = nmwi.PopFromModel<OutputLine>();
	m_upInputLine  = nmwi.PopFromModel<InputLine>();
}
