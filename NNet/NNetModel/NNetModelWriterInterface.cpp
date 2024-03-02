// NNetModelWriterInterface.cpp
//
// NNetModel

module;

#include <cassert>
#include <memory>

module NNetModel:NNetModelWriterInterface;

import Observable;
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

unique_ptr<Model> NNetModelWriterInterface::CreateNewModel(Observable * const pLockModelObservable)
{
	unique_ptr<Model> upModel { make_unique<Model>(pLockModelObservable) };
	m_pModel = upModel.get();
	return move(upModel);
}

void NNetModelWriterInterface::CreateInitialNobs()
{
	IoLinePair(MicroMeterPnt(400.0_MicroMeter, 500.0_MicroMeter)).Push(*this);
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

void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::SetScanArea(MicroMeterRect const &rect)
{
	m_pModel->SetScanArea(rect);
}
