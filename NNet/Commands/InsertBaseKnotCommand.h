// InsertBaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "BaseKnot.h"

template <BaseKnot_t T>
class InsertBaseKnotCommand : public Command
{
public:
	InsertBaseKnotCommand
	(
		NobId         const   idPipe, 
		MicroMeterPnt const & umSplitPoint 
	)
	  :	m_idPipe(idPipe),
		m_umSplitPoint(umSplitPoint)
	{ }

	~InsertBaseKnotCommand() {}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		if (! m_upBaseKnot)
		{ 
			m_pPipe2Split = nmwi.GetNobPtr<Pipe *>(m_idPipe);
			m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr();
			m_upBaseKnot  = make_unique<T>   (m_umSplitPoint);
			m_upPipeNew   = make_unique<Pipe>(m_pStartKnot, m_upBaseKnot.get());
			m_upBaseKnot->AddOutgoing(m_pPipe2Split);
			m_upBaseKnot->AddIncoming(m_upPipeNew.get());
		}
		m_pStartKnot->ReplaceOutgoing(m_pPipe2Split, m_upPipeNew.get());
		m_pPipe2Split->SetStartKnot(m_upBaseKnot.get());
		nmwi.Push2Model(move(m_upBaseKnot));
		nmwi.Push2Model(move(m_upPipeNew));
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_upPipeNew  = nmwi.PopFromModel<Pipe>();
		m_upBaseKnot = nmwi.PopFromModel<T>();
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
