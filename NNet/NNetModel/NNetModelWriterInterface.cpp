// NNetModelWriterInterface.cpp
//
// NNetModel

module;

#include <cassert>
#include <memory>

module NNetModel:NNetModelWriterInterface;

import Types;
import :IoLinePair;
import :NobType;
import :NobId;
import :Nob;
import :Knot;
import :Neuron;
import :BaseKnot;

using std::make_unique;
using std::unique_ptr;

void NNetModelWriterInterface::CreateInitialNobs()
{
	IoLinePair(*this, MicroMeterPnt(400.0_MicroMeter, 500.0_MicroMeter)).Push(*this);
}

Nob * NNetModelWriterInterface::GetNob(NobId const id)
{	
	m_pModel->CheckId(id);
	return m_pModel->GetNob(id);
}

BaseKnot & NNetModelWriterInterface::GetBaseKnot(NobId const id)
{
	BaseKnot * pBaseKnot { GetNobPtr<BaseKnot*>(id) };
	assert(pBaseKnot);
	return *pBaseKnot;
}

void NNetModelWriterInterface::SelectSubtree(BaseKnot & baseKnot, bool  const b) 
{ 
	m_pModel->SelectSubtree(baseKnot, b); 
}

void NNetModelWriterInterface::AddOutgoing   (NobId const id, Pipe& pipe) { GetBaseKnot(id).AddOutgoing(pipe); }
void NNetModelWriterInterface::AddIncoming   (NobId const id, Pipe& pipe) { GetBaseKnot(id).AddIncoming(pipe); }
void NNetModelWriterInterface::RemoveIncoming(NobId const id, Pipe& pipe) { GetBaseKnot(id).RemoveIncoming(pipe); }
void NNetModelWriterInterface::RemoveOutgoing(NobId const id, Pipe& pipe) { GetBaseKnot(id).RemoveOutgoing(pipe); }

void NNetModelWriterInterface::SelectNob(NobId const idNob, bool const bOn) 
{ 
	GetNobPtr<Nob *>(idNob)->Select(bOn); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::RemoveOrphans()
{
	GetUPNobs().Apply2All<Knot>                              
	(                                                       
		[this](Knot const & knot)
		{
			if (knot.IsOrphan())
				RemoveFromModel<Knot>(knot);
		} 
	); 
}

void NNetModelWriterInterface::SetPosDir(NobId const id, MicroMeterPosDir const & umPosDir)
{
	GetNobPtr<Nob *>(id)->SetPosDir(umPosDir);
}

void ConnectIncoming(Pipe & p, BaseKnot & b)
{
	b.AddIncoming(p);
	p.SetEndPnt (&b);
}

void ConnectOutgoing(Pipe & p, BaseKnot & b)
{
	b.AddOutgoing (p);
	p.SetStartPnt(&b);
}

void ConnectIoLine(IoLine & l, BaseKnot & b)
{
	Pipe & pipe { l.GetPipe() };
	if (l.IsOutputLine())
		ConnectIncoming(pipe, b);
	else
		ConnectOutgoing(pipe, b);
}
