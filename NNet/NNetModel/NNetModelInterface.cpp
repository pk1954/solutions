// NNetModelInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Shape.h"
#include "NNetModel.h"
#include "NNetModelInterface.h"

bool const NNetModelInterface::AnyShapesSelected( ) const
{
	return m_pModel->AnyShapesSelected( );
}

bool const NNetModelInterface::IsSelected( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) };
	return p ? p->IsSelected( ) : false; 
}

ShapeType const NNetModelInterface::GetShapeType( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) };
	return p ? p->GetShapeType( ) : ShapeType::Value::undefined; 
}

fHertz const NNetModelInterface::GetPulseFreq( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<InputNeuron const *>(id) };
	return p ? p->GetPulseFreq( ) : fHertz::NULL_VAL(); 
}

size_t const NNetModelInterface::GetNrOfSegments( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments( ) : 0; 
}

bool const NNetModelInterface::HasTriggerSound( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
	return p ? p->HasTriggerSound( ) : false; 
}

Hertz const NNetModelInterface::GetTriggerSoundFrequency( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
	return p ? p->GetTriggerSoundFrequency( ) : Hertz::NULL_VAL(); 
}

MilliSecs const NNetModelInterface::GetTriggerSoundDuration ( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
	return p ? p->GetTriggerSoundDuration( ) : MilliSecs::NULL_VAL(); 
}

mV const NNetModelInterface::GetVoltage( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->GetVoltage( ) : mV::NULL_VAL(); 
}

mV const NNetModelInterface::GetVoltage( ShapeId const id, MicroMeterPoint const & umPoint ) const
{
	auto p { m_pModel->GetShapeConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltage( umPoint ) : mV::NULL_VAL(); 
}

size_t const NNetModelInterface::GetNrOfOutgoingConnections( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfOutgoingConnections( ) : -1;
}

size_t const NNetModelInterface::GetNrOfIncomingConnections( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->GetNrOfIncomingConnections( ) : -1;
}

bool const NNetModelInterface::HasIncoming( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->HasIncoming( ) : false; 
}

bool const NNetModelInterface::HasOutgoing( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->HasOutgoing( ) : false; 
}

MicroMeterRect const NNetModelInterface::GetEnclosingRect( ) const
{
	return m_pModel->GetEnclosingRect( );
}

fMicroSecs NNetModelInterface::GetSimulationTime( ) const 
{ 
	return m_pModel->GetSimulationTime( ); 
}

bool const NNetModelInterface::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( idSrc == idDst )
		return false;

	if ( (idSrc == NO_SHAPE) || (idDst == NO_SHAPE) )
		return false;

	if ( isConnectedTo( idSrc, idDst ) )  // if already connected we cannot connect again
		return false;

	ShapeType const typeSrc { GetShapeType( idSrc ) };

	if ( typeSrc.IsPipeType() )               // only BaseKnots can be connected
		return false;                         // to other shapes

	if ( typeSrc.IsInputNeuronType() )        // input neuron cannot connect to anything
		return false;                         // try the other way round

	ShapeType const typeDst { GetShapeType( idDst ) };

	if ( typeDst.IsPipeType( ) )
	{
		if ( typeSrc.IsInputNeuronType() )    // cannot insert input neuron
			return false;                     // into Pipe
	}
	else if ( typeDst.IsAnyNeuronType() )
	{
		if ( typeSrc.IsAnyNeuronType() )      // neurons cannot connect directly 
			return false;                     // to other neurons

		if ( typeDst.IsInputNeuronType() && HasIncoming( idSrc ) )  // cannot connect incoming dendrite
			return false;                                           // to input neuron

		if ( GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) > 1 ) // neurons can not not have 
			return false;                                                                    // more than one axon
	}

	return true;
}

bool const NNetModelInterface::isConnectedTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( GetShapeType( idSrc ).IsPipeType() )
		return isConnectedToPipe( idDst, idSrc );
	if ( GetShapeType( idDst ).IsPipeType() )
		return isConnectedToPipe( idSrc, idDst );
	else
		return false;
}

bool const NNetModelInterface::isConnectedToPipe( ShapeId const idShape, ShapeId const idPipe ) const
{
	return (idShape == m_pModel->GetStartKnotId(idPipe)) || (idShape == m_pModel->GetEndKnotId(idPipe));
}

unsigned long const NNetModelInterface::GetNrOfPipes       ( ) const { return Pipe       ::GetCounter(); }
unsigned long const NNetModelInterface::GetNrOfShapes      ( ) const { return Shape      ::GetCounter(); }
unsigned long const NNetModelInterface::GetNrOfKnots       ( ) const { return Knot       ::GetCounter(); }
unsigned long const NNetModelInterface::GetNrOfInputNeurons( ) const { return InputNeuron::GetCounter(); }
unsigned long const NNetModelInterface::GetNrOfNeurons     ( ) const { return Neuron     ::GetCounter() - GetNrOfInputNeurons(); }

void NNetModelInterface::DrawExterior
( 
	ShapeId        const   id,
	DrawContext    const & context,
	tHighlightType const   highlightType
) const
{
	if ( Shape const * const p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawExterior( context, highlightType );
}

void NNetModelInterface::DrawInterior
( 
	ShapeId     const   id, 
	DrawContext const & context
) 
	const
{
	if ( auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawInterior( context );
}

void NNetModelInterface::LockModelShared() const
{ 
	m_pModel->LockModelShared();
}

void NNetModelInterface::UnlockModelShared() const
{ 
	m_pModel->UnlockModelShared();
}
