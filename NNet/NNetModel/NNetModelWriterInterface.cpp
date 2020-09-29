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
	InputNeuron * const pInputNeuron { NewBaseKnot<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewBaseKnot<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	Pipe        * const pNewPipe     { NewPipe( pInputNeuron, pNeuron ) };
	pInputNeuron->m_connections.AddOutgoing( pNewPipe );
	pNeuron     ->m_connections.AddIncoming( pNewPipe );
	Store2Model( pInputNeuron );
	Store2Model( pNeuron );
	Store2Model( pNewPipe );
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

Pipe * const NNetModelWriterInterface::NewPipe( BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd )
{
	Pipe * const pPipe { new Pipe( pKnotStart, pKnotEnd ) };
	pPipe->SetId( m_pModel->NewShapeListSlot( ) );
	return pPipe;
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

void NNetModelWriterInterface::GetShapeList( ShapeList & dst, ShapeCrit const & selector ) const
{
	Apply2All<Shape>( [&]( Shape & s ) { if ( selector(s) ) dst.Add( & s ); } );
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
