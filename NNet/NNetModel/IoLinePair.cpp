// IoLinePair.cpp
//
// NNetModel

module NNetModel:IoLinePair;

import std;
import Types;
import :NNetModelWriterInterface;
import :Pipe;
import :PosNob;
import :InputLine;
import :OutputLine;

using std::unique_ptr;
using std::make_unique;

IoLinePair::IoLinePair(MicroMeterPnt const & pos)
  :	m_upInputLine (make_unique<InputLine> (pos - m_umOffset)),
	m_upOutputLine(make_unique<OutputLine>(pos + m_umOffset)),
	m_upPipe      (make_unique<Pipe>())
{
	m_upOutputLine->SetPipe(m_upPipe.get());
	m_upInputLine ->SetPipe(m_upPipe.get());
	m_upPipe->SetStartPnt(m_upInputLine.get());
	m_upPipe->SetEndPnt  (m_upOutputLine.get());
	m_upPipe->PosChanged();
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
