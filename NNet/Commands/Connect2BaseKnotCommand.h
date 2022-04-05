// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "BaseKnot.h"
#include "Knot.h"

class Connect2BaseKnotCommand : public NNetCommand
{
public:
	Connect2BaseKnotCommand
	(
		NobId const idSrc,
		NobId const idDst
	)
	  :	m_baseKnotSrc(*m_pNMWI->GetNobPtr<BaseKnot *>(idSrc)),
		m_baseKnotDst(*m_pNMWI->GetNobPtr<BaseKnot *>(idDst))
	{ 
		size_t nrIn  = m_baseKnotSrc.GetNrOfInConns () + m_baseKnotDst.GetNrOfInConns ();
		size_t nrOut = m_baseKnotSrc.GetNrOfOutConns() + m_baseKnotDst.GetNrOfOutConns();

		if (m_baseKnotSrc.IsKnot() && m_baseKnotDst.IsKnot())
			m_upResult = make_unique<Knot>(m_baseKnotDst);
		else if (nrOut == 0)
			m_upResult = make_unique<OutputNeuron>(m_baseKnotDst);
		else if (nrOut == 1)
		{
			if (nrIn == 0)
				m_upResult = make_unique<InputNeuron>(m_pNMWI->StdSigGen(), m_baseKnotDst);
			else 
				m_upResult = make_unique<Neuron>(m_baseKnotDst);
		}
		else 
			assert(false);

		m_upResult->AddIncoming(m_baseKnotSrc);
		m_upResult->AddOutgoing(m_baseKnotSrc);
	}

	void Do() final
	{
		assert(m_upResult);
		m_upResult->Reconnect();
		m_upBaseKnotDst = m_pNMWI->ReplaceInModel<BaseKnot>(move(m_upResult));
		m_upBaseKnotSrc = m_pNMWI->RemoveFromModel<BaseKnot>(m_baseKnotSrc); 
		assert(m_upBaseKnotSrc);
		assert(m_upBaseKnotDst);
	}

	void Undo() final
	{
		m_upBaseKnotDst->Reconnect();
		m_upBaseKnotSrc->Reconnect();
		m_upResult = m_pNMWI->ReplaceInModel<BaseKnot>(move(m_upBaseKnotDst));
		m_pNMWI->Restore2Model(move(m_upBaseKnotSrc));
	}

private:

	BaseKnot const & m_baseKnotSrc;
	BaseKnot const & m_baseKnotDst;

	unique_ptr<BaseKnot> m_upResult      { nullptr };
	unique_ptr<BaseKnot> m_upBaseKnotSrc { nullptr };
	unique_ptr<BaseKnot> m_upBaseKnotDst { nullptr };
};
