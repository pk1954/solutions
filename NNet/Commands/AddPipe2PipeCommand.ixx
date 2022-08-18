// AddPipe2PipeCommand.ixx
//
// Commands

module;

#include <cassert> 
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NobType.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "NNetCommand.h"

export module AddPipe2PipeCommand;

import MoreTypes;

using std::unique_ptr;

export class AddPipe2PipeCommand : public NNetCommand
{
public:
	AddPipe2PipeCommand
	(
		NobId         const   idPipe,
		MicroMeterPnt const& pos,
		NobType       const   type
	)
		: m_idPipe(idPipe)
	{
		assert(type.IsIoLineType());

		m_pPipeOld = m_pNMWI->GetNobPtr<Pipe*>(m_idPipe);
		m_pStartKnot = m_pPipeOld->GetStartKnotPtr();
		m_pEndKnot = m_pPipeOld->GetEndKnotPtr();
		m_upKnotInsert = make_unique<Knot>(pos);
		m_upPipeNew1 = make_unique<Pipe>(m_pStartKnot, m_upKnotInsert.get());
		m_upPipeNew2 = make_unique<Pipe>(m_upKnotInsert.get(), m_pEndKnot);

		m_upKnotInsert->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());

		m_upKnotInsert->AddIncoming(*m_upPipeNew1.get());
		m_upKnotInsert->AddOutgoing(*m_upPipeNew2.get());

		if (type.IsInputLineType())
		{
			m_upExtPoint = make_unique<InputLine>(m_pNMWI->StdSigGen(), pos - m_pNMWI->OrthoVector(m_idPipe));
			m_upPipeOrtho = make_unique<Pipe>(m_upExtPoint.get(), m_upKnotInsert.get());
			m_upExtPoint->AddOutgoing(*m_upPipeOrtho.get());
			m_upKnotInsert->AddIncoming(*m_upPipeOrtho.get());
		}
		else
		{
			m_upExtPoint = make_unique<OutputLine>(pos + m_pNMWI->OrthoVector(m_idPipe));
			m_upPipeOrtho = make_unique<Pipe>(m_upKnotInsert.get(), m_upExtPoint.get());
			m_upExtPoint->AddIncoming(*m_upPipeOrtho.get());
			m_upKnotInsert->AddOutgoing(*m_upPipeOrtho.get());
		}
	}

	~AddPipe2PipeCommand() final = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upKnotInsert));
		m_pNMWI->Push2Model(move(m_upPipeNew1));
		m_pNMWI->Push2Model(move(m_upPipeNew2));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upExtPoint));

		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipeOld));

		m_upExtPoint = m_pNMWI->PopFromModel<IoLine>();
		m_upPipeOrtho = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew2 = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew1 = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotInsert = m_pNMWI->PopFromModel<Knot>();

		m_pEndKnot->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);
	}

private:
	Pipe     * m_pPipeOld{ nullptr };
	BaseKnot * m_pStartKnot{ nullptr };
	BaseKnot * m_pEndKnot{ nullptr };
	unique_ptr<Pipe>   m_upPipeOld{ nullptr };
	unique_ptr<Pipe>   m_upPipeNew1{ nullptr };
	unique_ptr<Pipe>   m_upPipeNew2{ nullptr };
	unique_ptr<Pipe>   m_upPipeOrtho{ nullptr };
	unique_ptr<Knot>   m_upKnotInsert{ nullptr };
	unique_ptr<IoLine> m_upExtPoint{ nullptr };
	NobId        const m_idPipe;
};
