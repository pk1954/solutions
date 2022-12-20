// SelectNobCommand.ixx
//
// Commands

module;

#include <cassert>

export module SelectNobCommand;

import BoolOp;
import SelectionCommand;
import NNetModel;

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
		Nob* pNob    { m_pNMWI->GetNobPtr<Nob*>(m_idNob) };
		bool bSelect { ApplyOp2(pNob->IsSelected(), m_op) };
		pNob->Select(bSelect);
		if (pNob->IsPipe())
		{
			Pipe& pipe { Cast2Pipe(*pNob) };
			pipe.GetStartNobPtr()->Select(bSelect);
			pipe.GetEndNobPtr()->Select(bSelect);
		}
		if (pNob->IsSynapse())
		{
			Pipe& pipe { *Cast2Synapse(pNob)->GetAddPipe() };
			pipe.Select(bSelect);
			pipe.GetStartNobPtr()->Select(bSelect);
		}
		if (pNob->IsInputLine())
		{
			Pipe& pipe { *Cast2InputLine(pNob)->GetPipe() };
			pipe.Select(bSelect);
			pipe.GetEndNobPtr()->Select(bSelect);
		}
		if (pNob->IsOutputLine())
		{
			Pipe& pipe { *Cast2OutputLine(pNob)->GetPipe() };
			pipe.Select(bSelect);
			pipe.GetStartNobPtr()->Select(bSelect);
		}
	}

private:
	NobId   const m_idNob;
	tBoolOp const m_op;
};
