// Connect2BaseKnotCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module Connect2BaseKnotCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class Connect2BaseKnotCommand : public NNetCommand
{
public:
	Connect2BaseKnotCommand
	(
		NobId          const idSrc,
		NobId          const idDst,
		ConnectionType const cType
	)
	  : m_baseKnotSrc(*m_pNMWI->GetNobPtr<BaseKnot*>(idSrc)),
		m_baseKnotDst(*m_pNMWI->GetNobPtr<BaseKnot*>(idDst))
	{
		MicroMeterPnt pos { m_baseKnotDst.GetPos() };

		using enum ConnectionType;

		switch (cType)
		{
		    case ct_fork:       m_upResult = make_unique<Fork>      (pos); break;
			case ct_synapse:	m_upResult = make_unique<Synapse>   (pos); break;
			case ct_neuron:		m_upResult = make_unique<Neuron>    (pos); break;
			case ct_outputline:	m_upResult = make_unique<OutputLine>(pos); break;
			default: assert(false);
		}
		m_upResult->AddIncoming(m_baseKnotSrc);
		m_upResult->AddOutgoing(m_baseKnotSrc);
	}

	void Do() final
	{
		assert(m_upResult);
		m_upResult->Reconnect();
		m_upBaseKnotDst = m_pNMWI->ReplaceInModel <BaseKnot>(move(m_upResult));
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
