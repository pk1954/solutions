// Connect2PipeCommand.ixx
//
// Commands

module;

#include <cassert>
#include "NNetModelWriterInterface.h"
#include "BaseKnot.h"
#include "Pipe.h"

export module Connect2PipeCommand;

import Types;
import NobId;
import NNetCommand;
import Knot;

using std::unique_ptr;
using std::make_unique;

export class Connect2PipeCommand : public NNetCommand
{
public:
	Connect2PipeCommand
	(
		NobId const idIoLine,
		NobId const idPipe
	)
	  : m_idIoLine(idIoLine),
		m_idPipe(idPipe),
		m_pIoLine(m_pNMWI->GetNobPtr<BaseKnot*>(idIoLine)),
		m_pPipeOld(m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pStartKnot(m_pPipeOld->GetStartKnotPtr()),
		m_pEndKnot(m_pPipeOld->GetEndKnotPtr()),
		m_pos(m_pIoLine->GetPos())
	{
		assert(m_pIoLine->IsIoLine());

		m_upKnotInsert = make_unique<Knot>(m_pos);
		m_upPipeNew1 = make_unique<Pipe>(m_pStartKnot, m_pIoLine);
		m_upPipeNew2 = make_unique<Pipe>(m_pIoLine, m_pEndKnot);

		m_pIoLine->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());

		m_upKnotInsert->AddIncoming(*m_upPipeNew1.get());
		m_upKnotInsert->AddOutgoing(*m_upPipeNew2.get());
	}

	~Connect2PipeCommand() final = default;

	void Do() final
	{
		if (m_pIoLine->IsInputLine())
			m_pIoLine->Apply2AllOutPipes([this](Pipe& p) { ConnectOutgoing(p, *m_upKnotInsert.get()); });
		else
			m_pIoLine->Apply2AllInPipes([this](Pipe& p) { ConnectIncoming(p, *m_upKnotInsert.get()); });

		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upKnotInsert));
		m_pNMWI->Push2Model(move(m_upPipeNew1));
		m_pNMWI->Push2Model(move(m_upPipeNew2));

		m_upIoLine = m_pNMWI->RemoveFromModel<IoLine>(m_idIoLine);
		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipeOld));
		m_pNMWI->Restore2Model(move(m_upIoLine));

		m_upPipeNew2 = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew1 = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotInsert = m_pNMWI->PopFromModel<Knot>();

		m_pEndKnot->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);

		if (m_pIoLine->IsInputLine())
			m_pIoLine->Apply2AllOutPipes([this](Pipe& p) { p.SetStartKnot(m_pIoLine); });
		else
			m_pIoLine->Apply2AllInPipes([this](Pipe& p) { p.SetEndKnot(m_pIoLine); });
	}

private:
	NobId         const m_idIoLine;
	NobId         const m_idPipe;
	BaseKnot    * const m_pIoLine;
	Pipe        * const m_pPipeOld;
	BaseKnot    * const m_pStartKnot;
	BaseKnot    * const m_pEndKnot;
	MicroMeterPnt const m_pos;

	unique_ptr<Pipe>     m_upPipeNew1{};
	unique_ptr<Pipe>     m_upPipeNew2{};
	unique_ptr<Pipe>     m_upPipeOld{};
	unique_ptr<Knot>     m_upKnotInsert{};
	unique_ptr<BaseKnot> m_upIoLine{};
};
