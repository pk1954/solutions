// InsertBaseKnotCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "NNetCommand.h"

export module InsertBaseKnotCommand;

import MoreTypes;

export template <BaseKnot_t T>
class InsertBaseKnotCommand : public NNetCommand
{
public:
	InsertBaseKnotCommand
	(
		NobId         const   idPipe, 
		MicroMeterPnt const & umSplitPoint 
	)
	  :	m_idPipe(idPipe),
		m_umSplitPoint(umSplitPoint)
	{ 
		m_pPipe2Split = m_pNMWI->GetNobPtr<Pipe *>(m_idPipe);
		m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr();
		m_upBaseKnot  = make_unique<T>(m_umSplitPoint);
		m_upPipeNew   = make_unique<Pipe>(m_pStartKnot, m_upBaseKnot.get());
		m_upBaseKnot->AddOutgoing(*m_pPipe2Split);
		m_upBaseKnot->AddIncoming(*m_upPipeNew.get());
	}

	~InsertBaseKnotCommand() final = default;

	void Do() final 
	{ 
		m_pStartKnot->ReplaceOutgoing(m_pPipe2Split, m_upPipeNew.get());
		m_pPipe2Split->SetStartKnot(m_upBaseKnot.get());
		m_pNMWI->Push2Model(move(m_upBaseKnot));
		m_pNMWI->Push2Model(move(m_upPipeNew));
	}

	void Undo() final 
	{ 
		m_upPipeNew  = m_pNMWI->PopFromModel<Pipe>();
		m_upBaseKnot = m_pNMWI->PopFromModel<T>();
		m_pPipe2Split->SetStartKnot(m_pStartKnot);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew.get(), m_pPipe2Split);
	}

private:
	Pipe     * m_pPipe2Split { nullptr };
	BaseKnot * m_pStartKnot  { nullptr };

	unique_ptr<Pipe> m_upPipeNew;
	unique_ptr<T>    m_upBaseKnot;

	NobId         const m_idPipe;
	MicroMeterPnt const m_umSplitPoint; 
};
