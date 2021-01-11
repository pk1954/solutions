// NNetModelWriterInterface.cpp
//
// NNetModel

#include "stdafx.h"
#include "Neuron.h"
#include "NNetModelWriterInterface.h"

void NNetModelWriterInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
}

void NNetModelWriterInterface::Stop( )
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::CreateInitialShapes( )
{
	unique_ptr<InputNeuron> upInputNeuron { make_unique<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	unique_ptr<Neuron>      upNeuron      { make_unique<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	unique_ptr<Pipe>        upNewPipe     { make_unique<Pipe>( upInputNeuron.get(), upNeuron.get() ) };
	upInputNeuron->m_connections.AddOutgoing( upNewPipe.get() );
	upNeuron     ->m_connections.AddIncoming( upNewPipe.get() );
	GetShapes().Push( move(upInputNeuron) );
	GetShapes().Push( move(upNeuron) );       
	GetShapes().Push( move(upNewPipe) );      
}

Shape * const NNetModelWriterInterface::GetShape( ShapeId const id )     
{ 
	return const_cast<Shape *>(m_pModel->GetConstShape( id ) );
}

void NNetModelWriterInterface::SelectShape( ShapeId const idShape, tBoolOp const op ) 
{ 
	GetShapePtr<Shape *>( idShape )->Select( op ); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( id ) } )
		pNeuron->StopOnTrigger( tBoolOp::opToggle );
}

void NNetModelWriterInterface::SelectBeepers() 
{ 
	GetShapes().Apply2All<Neuron>
	( 
		[&](Neuron &n) 
		{ 
			if (n.HasTriggerSound()) 
				n.Select( tBoolOp::opTrue ); 
		} 
	); 
}

void NNetModelWriterInterface::RemoveOrphans( )
{
	GetShapes().Apply2All<Knot>                              
	(                                                        
		[&]( Knot const & knot )                  
		{
			if ( knot.IsOrphanedKnot() )
				RemoveFromModel<Knot>( knot.GetId() );
		} 
	); 
}