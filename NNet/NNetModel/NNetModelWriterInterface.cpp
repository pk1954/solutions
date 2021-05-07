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
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"

void NNetModelWriterInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
}

void NNetModelWriterInterface::Stop()
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::CreateInitialNobs()
{
	unique_ptr<InputNeuron> upInputNeuron { make_unique<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	unique_ptr<OutputNeuron>upOutputNeuron{ make_unique<OutputNeuron>( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	unique_ptr<Pipe>        upNewPipe     { make_unique<Pipe>( upInputNeuron.get(), upOutputNeuron.get() ) };
	upInputNeuron ->m_connections.AddOutgoing( upNewPipe.get() );
	upOutputNeuron->m_connections.AddIncoming( upNewPipe.get() );
	GetUPNobs().Push( move(upInputNeuron) );
	GetUPNobs().Push( move(upOutputNeuron) );       
	GetUPNobs().Push( move(upNewPipe) );      
}

Nob * const NNetModelWriterInterface::GetNob( NobId const id )     
{ 
	return const_cast<Nob *>(m_pModel->GetConstNob( id ) );
}

void NNetModelWriterInterface::SelectNob(NobId const idNob, bool const bOn) 
{ 
	GetNobPtr<Nob *>(idNob)->Select(bOn, false); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger( NobId const id )
{
	if ( Neuron * pNeuron { GetNobPtr<Neuron *>( id ) } )
		pNeuron->StopOnTrigger( tBoolOp::opToggle );
}

void NNetModelWriterInterface::SelectBeepers() 
{ 
	GetUPNobs().Apply2All<Neuron>
	( 
		[&](Neuron &n) 
		{ 
			if (n.HasTriggerSound()) 
				n.Select(true, false); 
		} 
	); 
}

void NNetModelWriterInterface::RemoveOrphans()
{
	GetUPNobs().Apply2All<Knot>                              
	(                                                        
		[&]( Knot const & knot )                  
		{
			if ( knot.IsOrphanedKnot() )
				RemoveFromModel<Knot>( knot );
		} 
	); 
}

void NNetModelWriterInterface::SetConnNeurons
(
	MicroMeterPointVector & umPntVector, 
	NobIdList     const & nobIds
)
{
	unsigned int ui = 0;
	nobIds.Apply2All
	(
		[&](NobId const & id)
		{
			ConnNeuron     * const pConnNeuron { GetNobPtr<ConnNeuron *>(id) };
			MicroMeterPosDir const posDir      { pConnNeuron->GetRawPosDir() };
			pConnNeuron->SetPosDir( umPntVector.GetPosDir(ui) );
			umPntVector.SetPosDir( ui, posDir );
			++ui;
		}
	);
}

void NNetModelWriterInterface::SetConnNeurons
(
	MicroMeterPointVector    const & umPntVector, 
	NobPtrList<ConnNeuron> const & nobPtrList
)
{
	unsigned int ui = 0;
	nobPtrList.Apply2All
	(
		[&](ConnNeuron & c)	
		{ c.SetPosDir( umPntVector.GetPosDir(ui++) ); }
	);
}
