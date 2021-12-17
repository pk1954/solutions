// NNetModelWriterInterface.cpp
//
// NNetModel

#include "stdafx.h"
#include "Pipe.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "NobIdList.h"
#include "MicroMeterPntVector.h"
#include "NNetModelWriterInterface.h"

//void NNetModelWriterInterface::Start(NNetModel * const pModel)
//{
//	m_pModel = pModel;
//}
//
//void NNetModelWriterInterface::Stop()
//{
//	m_pModel = nullptr;
//}

void NNetModelWriterInterface::CreateInitialNobs()
{
	unique_ptr<InputNeuron> upInputNeuron { make_unique<InputNeuron >(MicroMeterPnt(400.0_MicroMeter, 200.0_MicroMeter)) };
	unique_ptr<OutputNeuron>upOutputNeuron{ make_unique<OutputNeuron>(MicroMeterPnt(400.0_MicroMeter, 800.0_MicroMeter)) };
	unique_ptr<Pipe>        upNewPipe     { make_unique<Pipe>(upInputNeuron.get(), upOutputNeuron.get()) };
	upInputNeuron ->AddOutgoing(upNewPipe.get());
	upOutputNeuron->AddIncoming(upNewPipe.get());
	GetUPNobs().Push(move(upInputNeuron));
	GetUPNobs().Push(move(upOutputNeuron));       
	GetUPNobs().Push(move(upNewPipe));      
}

Nob * NNetModelWriterInterface::GetNob(NobId const id)
{ 
	return const_cast<Nob *>(m_pModel->GetConstNob(id));
}

void NNetModelWriterInterface::SelectNob(NobId const idNob, bool const bOn) 
{ 
	GetNobPtr<Nob *>(idNob)->Select(bOn); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger(NobId const id)
{
	if (Neuron * pNeuron { GetNobPtr<Neuron *>(id) })
		pNeuron->StopOnTrigger(tBoolOp::opToggle);
}

void NNetModelWriterInterface::SelectBeepers() 
{ 
	GetUPNobs().Apply2All<Neuron>
	(
		[](Neuron &n) 
		{ 
			if (n.HasTriggerSound()) 
				n.Select(true); 
		} 
	); 
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

void NNetModelWriterInterface::RecalcSignals()
{
	m_pModel->GetMonitorData().Apply2AllSignals
	(
		[](Signal & signal){ signal.Recalc(); }
	);
}
