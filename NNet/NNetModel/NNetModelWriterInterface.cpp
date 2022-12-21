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

void NNetModelWriterInterface::AddOutgoing(NobId const id, Pipe* p) { GetPosNob(id).AddOutgoing(p); }
void NNetModelWriterInterface::AddIncoming(NobId const id, Pipe* p) { GetPosNob(id).AddIncoming(p); }

//void NNetModelWriterInterface::SelectNob(NobId const idNob, bool const bOn) 
//{ 
//	GetNobPtr<Nob *>(idNob)->Select(bOn); 
//}
//
void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::SetPosDir(NobId const id, MicroMeterPosDir const & umPosDir)
{
	GetNobPtr<Nob *>(id)->SetPosDir(umPosDir);
}
