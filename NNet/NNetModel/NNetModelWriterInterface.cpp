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

void NNetModelWriterInterface::CreateInitialNobs()
{
	unique_ptr<InputNeuron> upInputNeuron { make_unique<InputNeuron >(MicroMeterPnt(400.0_MicroMeter, 200.0_MicroMeter)) };
	unique_ptr<OutputNeuron>upOutputNeuron{ make_unique<OutputNeuron>(MicroMeterPnt(400.0_MicroMeter, 800.0_MicroMeter)) };
	unique_ptr<Pipe>        upNewPipe     { make_unique<Pipe>(upInputNeuron.get(), upOutputNeuron.get(), GetParams()) };
	upInputNeuron ->AddOutgoing(upNewPipe.get());
	upOutputNeuron->AddIncoming(upNewPipe.get());
	GetUPNobs().Push(move(upInputNeuron));
	GetUPNobs().Push(move(upOutputNeuron));       
	GetUPNobs().Push(move(upNewPipe));
}

Nob * NNetModelWriterInterface::GetNob(NobId const id)
{	
	m_pModel->CheckId(id);
	return m_pModel->GetNob(id);
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

unique_ptr<BaseKnot> NNetModelWriterInterface::FixBaseKnot(NobId const id)
{
	BaseKnot const * pBaseKnot  { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	NobType  const   typeOld    { pBaseKnot->GetNobType() };
	size_t   const   nrInPipes  { pBaseKnot->GetNrOfIncomingConnections() };
	size_t   const   nrOutPipes { pBaseKnot->GetNrOfOutgoingConnections() };
	NobType          typeNew    { NobType::Value::undefined };

	if (nrOutPipes == 0)
	{
		if (nrInPipes > 0) // one or several inPipes
			typeNew = NobType::Value::outputNeuron;
	}
	else if (nrOutPipes == 1)
	{
		if ( nrInPipes == 0 )
			typeNew = NobType::Value::inputNeuron;
		else // one or several inPipes
			typeNew = typeOld.IsKnotType() ? NobType::Value::knot : NobType::Value::neuron;
	}
	else // more than one outPipe
	{
		if (nrInPipes <= 1)
			typeNew = NobType::Value::knot;
		else // more than one inPipe
			assert(false);
	}
	if (typeNew != typeOld)
	{
		unique_ptr<BaseKnot> upBaseKnotNew { };
		switch (typeNew.GetValue())
		{
			using enum NobType::Value;
			case knot:	       upBaseKnotNew = make_unique<Knot>        (*pBaseKnot); break;
			case neuron:	   upBaseKnotNew = make_unique<Neuron>      (*pBaseKnot); break;
			case inputNeuron:  upBaseKnotNew = make_unique<InputNeuron> (*pBaseKnot); break;
			case outputNeuron: upBaseKnotNew = make_unique<OutputNeuron>(*pBaseKnot); break;
			case undefined:	   break;
			default:           assert(false);
		}
		return upBaseKnotNew
               ? ReplaceInModel<BaseKnot>(move(upBaseKnotNew))
			   : RemoveFromModel<BaseKnot>(id);		
	}
	return unique_ptr<BaseKnot>();
}
