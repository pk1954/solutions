// InsertPosNobCommand.ixx
//
// Commands

module;

#include <memory>

export module InsertPosNobCommand;

import Types;
import NNetModel;
import SplitPipeCommand;

using std::unique_ptr;
using std::make_unique;
using std::pair;

export template <PosNob_t POS_NOB>
class InsertPosNobCommand : public SplitPipeCommand
{
public:
	InsertPosNobCommand
	(
		NobId         const   idPipe, 
		MicroMeterPnt const & umSplitPoint 
	)
      : SplitPipeCommand(idPipe)
	{
		m_upInsertNob = make_unique<POS_NOB>(umSplitPoint);
		SplitPipeCommand::InitSplit(*m_upInsertNob.get());
	}

	~InsertPosNobCommand() final = default;

	void Do() final 
	{ 
		SplitPipeCommand::Do();
		m_pNMWI->Push2Model(move(m_upInsertNob));
	}

	void Undo() final 
	{ 
		m_upInsertNob = m_pNMWI->PopFromModel<POS_NOB>();
		SplitPipeCommand::Undo();
	}

private:

	unique_ptr<POS_NOB> m_upInsertNob;
};
