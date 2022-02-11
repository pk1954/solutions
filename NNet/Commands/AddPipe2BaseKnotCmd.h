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

class AddPipe2BaseKnotCommand : public NNetCommand
{
public:
	AddPipe2BaseKnotCommand
	(
		NobId         const   id, 
		MicroMeterPnt const & pos,
		NobType       const   nobType
	)
		:	m_id(id),
		    m_type(nobType)
	{ 
		if (m_type.IsInputNeuronType())
			m_upNew = make_unique<InputNeuron>(pos);
		else
			m_upNew = make_unique<OutputNeuron>(pos);
		m_pOld   = m_pNMWI->GetNobPtr<BaseKnot *>(m_id);
		m_upPipe = MakePipe(m_pOld, m_upNew.get());
		m_upNew->AddPipe(m_upPipe.get(), switchIoType(m_type));
	}

	~AddPipe2BaseKnotCommand() final = default;

	void Do() final 
	{ 
		m_pOld->AddPipe(m_upPipe.get(), m_type);
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upNew));
		m_upOld = m_pNMWI->FixBaseKnot(m_id);
	}

	void Undo() final 
	{ 
		m_upNew  = m_pNMWI->PopFromModel<IoNeuron>();
		m_upPipe = m_pNMWI->PopFromModel<Pipe>();
		m_pNMWI->Restore2Model(move(m_upOld)); 
		m_pNMWI->RemovePipeFromBaseKnot(m_id, m_upPipe.get());
		m_pNMWI->FixBaseKnot(m_id);
	}

private:
	NobType switchIoType(NobType const type) const
	{
		assert(type.IsIoNeuronType());
		return type.IsInputNeuronType() 
			   ? NobType::Value::outputNeuron 
			   : NobType::Value::inputNeuron;
	}

	NobId          const m_id;
	NobType        const m_type;
	BaseKnot           * m_pOld   { nullptr };
	unique_ptr<BaseKnot> m_upOld  { };
	unique_ptr<IoNeuron> m_upNew  { };
	unique_ptr<Pipe>     m_upPipe { };
};
