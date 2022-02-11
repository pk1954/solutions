// Connect2PipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "BaseKnot.h"
#include "Pipe.h"

using std::make_unique;

class Connect2PipeCommand : public NNetCommand
{
public:
	Connect2PipeCommand
	(
		NobId const idIoNeuron,
		NobId const idPipe
	)
	:	m_idIoNeuron(idIoNeuron),
		m_idPipe    (idPipe),
		m_pIoNeuron (m_pNMWI->GetNobPtr<BaseKnot *>(idIoNeuron)),
		m_pPipeOld  (m_pNMWI->GetNobPtr<Pipe     *>(idPipe)),
		m_pStartKnot(m_pPipeOld->GetStartKnotPtr()),
		m_pEndKnot  (m_pPipeOld->GetEndKnotPtr()),
		m_pos       (m_pIoNeuron->GetPos())
	{
		assert(m_pIoNeuron.IsIoNeuron());

		m_upKnotInsert = make_unique<Knot>(m_pos);
		m_upPipeNew1   = MakePipe(m_pStartKnot, m_pIoNeuron);
		m_upPipeNew2   = MakePipe(m_pIoNeuron,  m_pEndKnot);

		m_pIoNeuron ->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());
	
		m_pIoNeuron->AddIncoming(m_upPipeNew1.get());
		m_pIoNeuron->AddOutgoing(m_upPipeNew2.get());
	}

	 ~Connect2PipeCommand() final = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot  ->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upKnotInsert ));
		m_pNMWI->Push2Model(move(m_upPipeNew1 ));
		m_pNMWI->Push2Model(move(m_upPipeNew2 ));

		m_upIoNeuron = m_pNMWI->RemoveFromModel<IoNeuron>(m_idIoNeuron);
		m_upPipeOld  = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipeOld));
		m_pNMWI->Restore2Model(move(m_upIoNeuron));

		m_upPipeNew2   = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew1   = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotInsert = m_pNMWI->PopFromModel<Knot>();

		m_pEndKnot  ->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);
	}

private:
	NobId          const m_idIoNeuron;
	NobId          const m_idPipe;
	BaseKnot     * const m_pIoNeuron;
	Pipe         * const m_pPipeOld;
	BaseKnot     * const m_pStartKnot;
	BaseKnot     * const m_pEndKnot;
	MicroMeterPnt  const m_pos; 

	unique_ptr<Pipe>     m_upPipeNew1   {};
	unique_ptr<Pipe>     m_upPipeNew2   {};
	unique_ptr<Pipe>     m_upPipeOld    {};
	unique_ptr<Knot>     m_upKnotInsert {};
	unique_ptr<BaseKnot> m_upIoNeuron   {};
};
