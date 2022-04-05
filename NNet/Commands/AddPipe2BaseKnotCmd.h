// AddPipe2BaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "Knot.h"

class AddPipe2BaseKnotCommand : public NNetCommand
{
public:
	AddPipe2BaseKnotCommand
	(
		NobId         const   id, 
		MicroMeterPnt const & pos,
		NobType       const   nobType
	)
		:	m_type(nobType),
     		m_baseKnotOld(*m_pNMWI->GetNobPtr<BaseKnot*>(id))
	{ 
		m_upPipe = make_unique<Pipe>();

		if (m_baseKnotOld.IsKnot())
			m_upBaseKnotNew = make_unique<Knot>(m_baseKnotOld.GetPos());
		else 
			m_upBaseKnotNew = make_unique<Neuron>(m_baseKnotOld.GetPos());

		if (m_type.IsInputNeuronType())
		{
			m_upExtPoint = make_unique<InputNeuron>(m_pNMWI->StdSigGen(), pos);
			ConnectOutgoing(*m_upPipe.get(), *m_upExtPoint.get());
			ConnectIncoming(*m_upPipe.get(), *m_upBaseKnotNew.get());
			m_upBaseKnotNew->SetOutgoing(m_baseKnotOld);
		}
		else
		{
			m_upExtPoint = make_unique<OutputNeuron>(pos);
			ConnectIncoming(*m_upPipe.get(), *m_upExtPoint.get());
			ConnectOutgoing(*m_upPipe.get(), *m_upBaseKnotNew.get());
			m_upBaseKnotNew->SetIncoming(m_baseKnotOld);
		}
	}

	~AddPipe2BaseKnotCommand() final = default;

	void Do() final 
	{ 
		if (m_type.IsInputNeuronType())
			m_baseKnotOld.Apply2AllOutPipes([this](Pipe &p){ ConnectOutgoing(p, *m_upBaseKnotNew.get()); });
		else
			m_baseKnotOld.Apply2AllInPipes([this](Pipe &p){ ConnectIncoming(p, *m_upBaseKnotNew.get()); });
		m_pNMWI->Push2Model(move(m_upBaseKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upExtPoint));
		m_upBaseKnotOld = m_pNMWI->RemoveFromModel<BaseKnot>(m_baseKnotOld);
	}

	void Undo() final 
	{ 
		m_upExtPoint    = m_pNMWI->PopFromModel<IoNeuron>();
		m_upPipe        = m_pNMWI->PopFromModel<Pipe>();
		m_upBaseKnotNew = m_pNMWI->PopFromModel<BaseKnot>();
		if (m_type.IsInputNeuronType())
			m_baseKnotOld.Apply2AllOutPipes([this](Pipe &p){ ConnectOutgoing(p, *m_upBaseKnotOld.get()); });
		else
			m_baseKnotOld.Apply2AllInPipes([this](Pipe &p){ ConnectIncoming(p, *m_upBaseKnotOld.get()); });
		m_pNMWI->Restore2Model(move(m_upBaseKnotOld)); 
	}

private:

	NobType        const m_type;
	BaseKnot const     & m_baseKnotOld;
	unique_ptr<BaseKnot> m_upBaseKnotOld { };
	unique_ptr<BaseKnot> m_upBaseKnotNew { };
	unique_ptr<IoNeuron> m_upExtPoint    { };
	unique_ptr<Pipe>     m_upPipe        { };
};
