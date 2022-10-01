// NewIoLinePairCmd.ixx
//
// Commands

export module NewIoLinePairCmd;

import Types;
import NNetCommand;
import NNetModel;

export class NewIoLinePairCmd : public NNetCommand
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
