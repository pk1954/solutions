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
import :PosNob;

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

PosNob & NNetModelWriterInterface::GetPosNob(NobId const id)
{
	PosNob * pPosNob { GetNobPtr<PosNob*>(id) };
	assert(pPosNob);
	return *pPosNob;
}

void NNetModelWriterInterface::SelectSubtree(PosNob & posNob, bool  const b) 
{ 
	m_pModel->SelectSubtree(posNob, b); 
}

void NNetModelWriterInterface::AddOutgoing   (NobId const id, Pipe& pipe) { GetPosNob(id).AddOutgoing(pipe); }
void NNetModelWriterInterface::AddIncoming   (NobId const id, Pipe& pipe) { GetPosNob(id).AddIncoming(pipe); }
void NNetModelWriterInterface::RemoveIncoming(NobId const id, Pipe& pipe) { GetPosNob(id).RemoveIncoming(pipe); }
void NNetModelWriterInterface::RemoveOutgoing(NobId const id, Pipe& pipe) { GetPosNob(id).RemoveOutgoing(pipe); }

void NNetModelWriterInterface::SelectNob(NobId const idNob, bool const bOn) 
{ 
	GetNobPtr<Nob *>(idNob)->Select(bOn); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::SetPosDir(NobId const id, MicroMeterPosDir const & umPosDir)
{
	GetNobPtr<Nob *>(id)->SetPosDir(umPosDir);
}

//void ConnectIncoming(Pipe & pipe, PosNob & posNob)
//{
//	posNob.AddIncoming(pipe);
//	pipe.SetEndPnt (&posNob);
//}
//
//void ConnectOutgoing(Pipe & pipe, PosNob & posNob)
//{
//	posNob.AddOutgoing(pipe);
//	pipe.SetStartPnt(&posNob);
//}
