// SelectNobCommand.ixx
//
// Commands

module;

#include <cassert>
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "BaseKnot.h"

export module SelectNobCommand;

import BoolOp;
import SelectionCommand;

export class SelectNobCommand : public SelectionCommand
{
public:
	SelectNobCommand
	(
		NobId   const id,
		tBoolOp const op
	)
		: m_idNob(id),
		m_op(op)
	{
		assert(IsDefined(m_idNob));
	}

	void Do() final
	{
		SelectionCommand::Do();
		Nob* pNob{ m_pNMWI->GetNobPtr<Nob*>(m_idNob) };
		pNob->Select(ApplyOp2(pNob->IsSelected(), m_op));
	}

private:
	NobId   const m_idNob;
	tBoolOp const m_op;
};
