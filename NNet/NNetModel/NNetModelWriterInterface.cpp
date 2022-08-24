// NNetModelWriterInterface.cpp
//
// NNetModel

#include <cassert>
#include "Pipe.h"
#include "Knot.h"
#include "Neuron.h"
#include "IoLinePair.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "NobIdList.h"
#include "NNetModelWriterInterface.h"

void NNetModelWriterInterface::CreateInitialNobs()
{
	IoLinePair(*this, MicroMeterPnt(400.0_MicroMeter, 500.0_MicroMeter)).Push(*this);
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

unique_ptr<BaseKnot> NNetModelWriterInterface::FixBaseKnot(NobId const id)
{
	BaseKnot const * pBaseKnot { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };

	if (pBaseKnot == nullptr)
		return unique_ptr<BaseKnot>();

	size_t const nrInPipes  { pBaseKnot->GetNrOfInConns() };
	size_t const nrOutPipes { pBaseKnot->GetNrOfOutConns() };
	NobType      typeNew    { NobType::Value::undefined };

	if (nrOutPipes == 0)
	{
		if (nrInPipes > 0) // one or several inPipes
			typeNew = NobType::Value::outputLine;
	}
	else if (nrOutPipes == 1)
	{
		if ( nrInPipes == 0 )
			typeNew = NobType::Value::inputLine;
		else // one or several inPipes
			typeNew = pBaseKnot->IsNeuron() ? NobType::Value::neuron : NobType::Value::knot;
	}
	else // more than one outPipe
	{
		if (nrInPipes <= 1)
			typeNew = NobType::Value::knot;
		else // more than one inPipe
			assert(false);
	}
	if (typeNew !=pBaseKnot->GetNobType())
	{
		unique_ptr<BaseKnot> upBaseKnotNew { };
		switch (typeNew.GetValue())
		{
			using enum NobType::Value;
			case knot:	     upBaseKnotNew = make_unique<Knot>        (*pBaseKnot); break;
			case neuron:	 upBaseKnotNew = make_unique<Neuron>      (*pBaseKnot); break;
			case inputLine:  upBaseKnotNew = make_unique<InputLine> (StdSigGen(), *pBaseKnot); break;
			case outputLine: upBaseKnotNew = make_unique<OutputLine>(*pBaseKnot); break;
			case undefined:	 break;
			default:         assert(false);
		}
 		return upBaseKnotNew
               ? ReplaceInModel<BaseKnot>(move(upBaseKnotNew))
			   : RemoveFromModel<BaseKnot>(id);		
	}
	return unique_ptr<BaseKnot>();
}

void ConnectIncoming(Pipe & p, BaseKnot & b)
{
	b.AddIncoming(p);
	p.SetEndKnot (&b);
}

void ConnectOutgoing(Pipe & p, BaseKnot & b)
{
	b.AddOutgoing (p);
	p.SetStartKnot(&b);
}
