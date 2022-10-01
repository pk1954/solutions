// IoLinePair.ixx
//
// Commands

module;

#include <memory>

export module NNetModel:IoLinePair;

import Types;

import :NNetModelWriterInterface;
import :InputLine;
import :OutputLine;
import :Pipe;

using std::unique_ptr;

export class IoLinePair
{
public:
	explicit IoLinePair(NNetModelWriterInterface &,	MicroMeterPnt const &);

	~IoLinePair();

	void Push(NNetModelWriterInterface &);
	void Pop (NNetModelWriterInterface &);

private:
	inline static MicroMeterPnt const m_umOffset{ 0.0_MicroMeter, 300.0_MicroMeter };

	unique_ptr<InputLine>  m_upInputLine;
	unique_ptr<OutputLine> m_upOutputLine;
	unique_ptr<Pipe>       m_upPipe;
};