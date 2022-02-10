// AddOutgoing2BaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "OutputNeuron.h"
#include "BaseKnot.h"
#include "Pipe.h"

class AddOutgoing2BaseKnotCommand : public NNetCommand
{
public:
	AddOutgoing2BaseKnotCommand
	(
		NobId         const   id, 
		MicroMeterPnt const & pos 
	)
	  :	m_id(id)
	{ 
		m_upNew  = make_unique<OutputNeuron>(pos);
		m_pOld   = m_pNMWI->GetNobPtr<BaseKnot *>(m_id);
		m_upPipe = MakePipe(m_pOld, m_upNew.get());
		m_upNew->AddIncoming(m_upPipe.get());
	}

	~AddOutgoing2BaseKnotCommand() final = default;

	void Do() final 
	{ 
		m_pOld->AddOutgoing(m_upPipe.get());
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upNew));
		m_upOld = m_pNMWI->FixBaseKnot(m_id);
	}

	void Undo() final 
	{ 
		m_upNew  = m_pNMWI->PopFromModel<OutputNeuron>();
		m_upPipe = m_pNMWI->PopFromModel<Pipe>();
		m_pNMWI->Restore2Model(move(m_upOld)); 
		m_pNMWI->RemovePipeFromBaseKnot(m_id, m_upPipe.get());
		m_pNMWI->FixBaseKnot(m_id);
	}

private:
	NobId              const m_id;
	BaseKnot               * m_pOld   { nullptr };
	unique_ptr<BaseKnot>     m_upOld  { };
	unique_ptr<OutputNeuron> m_upNew  { };
	unique_ptr<Pipe>         m_upPipe { };
};
