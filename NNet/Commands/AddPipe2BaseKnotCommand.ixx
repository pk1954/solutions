// AddPipe2BaseKnotCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NobType.h"
#include "Neuron.h"
#include "InputLine.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "Knot.h"

export module AddPipe2BaseKnotCommand;

import MoreTypes;
import NobId;
import NNetCommand;
import IoLine;
import OutputLine;

export class AddPipe2BaseKnotCommand : public NNetCommand
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

		if (m_baseKnotOld.IsNeuron())
			m_upBaseKnotNew = make_unique<Neuron>(m_baseKnotOld.GetPos());
		else 
			m_upBaseKnotNew = make_unique<Knot>(m_baseKnotOld.GetPos());

		if (m_type.IsInputLineType())
		{
			m_upExtPoint = make_unique<InputLine>(m_pNMWI->StdSigGen(), pos);
			ConnectOutgoing(*m_upPipe.get(), *m_upExtPoint.get());
			ConnectIncoming(*m_upPipe.get(), *m_upBaseKnotNew.get());
			m_upBaseKnotNew->SetOutgoing(m_baseKnotOld);
		}
		else
		{
			m_upExtPoint = make_unique<OutputLine>(pos);
			ConnectIncoming(*m_upPipe.get(), *m_upExtPoint.get());
			ConnectOutgoing(*m_upPipe.get(), *m_upBaseKnotNew.get());
			m_upBaseKnotNew->SetIncoming(m_baseKnotOld);
		}
	}

	~AddPipe2BaseKnotCommand() final = default;

	void Do() final 
	{ 
		if (m_type.IsInputLineType())
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
		m_upExtPoint    = m_pNMWI->PopFromModel<IoLine>();
		m_upPipe        = m_pNMWI->PopFromModel<Pipe>();
		m_upBaseKnotNew = m_pNMWI->PopFromModel<BaseKnot>();
		if (m_type.IsInputLineType())
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
	unique_ptr<IoLine>   m_upExtPoint    { };
	unique_ptr<Pipe>     m_upPipe        { };
};
