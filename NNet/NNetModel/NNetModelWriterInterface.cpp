// NNetModelWriterInterface.cpp
//
// NNetModel

module;

#include <ctime>
#include <chrono>
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
using std::chrono::system_clock;

unique_ptr<Model> NNetModelWriterInterface::CreateNewModel()
{
	unique_ptr<Model> upModel { make_unique<Model>() };
	m_pModel = upModel.get();
	return upModel;
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

void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::AddEvent(EventType const& type)
{
	switch (type)
	{
	case EventType::stimulus:
		m_pModel->m_events.push_back(make_unique<StimulusEvent>(GetSigGenIdSelected()));
		break;
	case EventType::startScan:
		m_pModel->m_events.push_back(make_unique<StartScanEvent>());
		break;
	case EventType::stopScan:
		m_pModel->m_events.push_back(make_unique<StopScanEvent>());
		break;
	}
}

void NNetModelWriterInterface::AddEvent // for model import
(
	EventType const& type,
	fMicroSecs const usTimeStamp
)
{
	switch (type)
	{
	case EventType::startScan:
		m_pModel->m_events.push_back(make_unique<StartScanEvent>(usTimeStamp));
		break;
	case EventType::stopScan:
		m_pModel->m_events.push_back(make_unique<StopScanEvent>(usTimeStamp));
		break;
	default:
		assert(false);
	}
}

void NNetModelWriterInterface::AddStimulusEvent  // for model import
(
	fMicroSecs const usTimeStamp,
	SigGenId   const idSigGen
)
{
	m_pModel->m_events.push_back(make_unique<StimulusEvent>(usTimeStamp, idSigGen));
}
