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
	unique_ptr<InputNeuron> upInputNeuron { NewBaseKnot<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	unique_ptr<Neuron>      upNeuron      { NewBaseKnot<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	unique_ptr<Pipe>        upNewPipe     { NewPipe( upInputNeuron.get(), upNeuron.get() ) };
	upInputNeuron->m_connections.AddOutgoing( upNewPipe.get() );
	upNeuron     ->m_connections.AddIncoming( upNewPipe.get() );
	Add2Model( move(upInputNeuron) );
	Add2Model( move(upNeuron) );       
	Add2Model( move(upNewPipe) );      
	StaticModelChanged( );
}

void NNetModelWriterInterface::StaticModelChanged( ) 
{ 
	m_pModel->StaticModelChanged(); 
}

Shape * const NNetModelWriterInterface::GetShape( ShapeId const id )     
{ 
	return const_cast<Shape *>(m_pModel->GetConstShape( id ) );
}

void NNetModelWriterInterface::IncShapeList( long const lNrOfShapes ) 
{ 
	m_pModel->IncShapeList( lNrOfShapes );
}

void NNetModelWriterInterface::SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
{	
	m_pModel->SetShapeErrorHandler( pHandler );
}

ShapeId const NNetModelWriterInterface::newShapeListSlot( )
{
	return m_pModel->NewShapeListSlot();
}

unique_ptr<Pipe> NNetModelWriterInterface::NewPipe( Pipe const & pipeSrc )
{
	unique_ptr<Pipe> upPipe { make_unique<Pipe>( pipeSrc ) };
	upPipe->SetId( m_pModel->NewShapeListSlot( ) );
	return move(upPipe);
}

unique_ptr<Pipe> NNetModelWriterInterface::NewPipe( BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd )
{
	unique_ptr<Pipe> upPipe { make_unique<Pipe>( pKnotStart, pKnotEnd ) };
	upPipe->SetId( m_pModel->NewShapeListSlot( ) );
	return move(upPipe);
}

void NNetModelWriterInterface::MarkShape( ShapeId const idShape, tBoolOp const op ) 
{ 
	GetShapePtr<Shape *>( idShape )->Mark( op ); 
}

void NNetModelWriterInterface::SelectShape( ShapeId const idShape, tBoolOp const op ) 
{ 
	GetShapePtr<Shape *>( idShape )->Select( op ); 
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( id ) } )
	{
		m_pModel->ToggleStopOnTrigger( pNeuron );
	}
}

void NNetModelWriterInterface::SelectBeepers() 
{ 
	Apply2All<Neuron>
	( 
		[&](Neuron &n) 
		{ 
			if (n.HasTriggerSound()) 
				n.Select( tBoolOp::opTrue ); 
		} 
	); 
}
