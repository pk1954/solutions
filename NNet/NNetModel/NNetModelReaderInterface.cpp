// NNetModelReaderInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Shape.h"
#include "NNetModel.h"
#include "NNetModelReaderInterface.h"

bool const NNetModelReaderInterface::IsEqual( NNetModel const & pModel) const
{
	return m_pModel->IsEqual( pModel );
}

void NNetModelReaderInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
}

void NNetModelReaderInterface::Stop( )
{
	m_pModel = nullptr;
}

bool const NNetModelReaderInterface::AnyShapesSelected( ) const
{
	return m_pModel->AnyShapesSelected( );
}

bool const NNetModelReaderInterface::IsSelected( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) };
	return p ? p->IsSelected( ) : false; 
}

MicroMeterPoint const NNetModelReaderInterface::GetShapePos( ShapeId const id ) const
{
	return m_pModel->GetShapePos( id );
}

ShapeType const NNetModelReaderInterface::GetShapeType( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) };
	return p ? p->GetShapeType( ) : ShapeType::Value::undefined; 
}

fHertz const NNetModelReaderInterface::GetPulseFrequency( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<InputNeuron const *>(id) };
	return p ? p->GetPulseFrequency( ) : fHertz::NULL_VAL(); 
}

size_t const NNetModelReaderInterface::GetNrOfSegments( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments( ) : 0; 
}

SoundDescr const NNetModelReaderInterface::GetTriggerSound( ShapeId const id ) const
{
	return m_pModel->GetShapeConstPtr<Neuron const *>(id)->GetTriggerSound( ); 
}

//bool const NNetModelReaderInterface::HasTriggerSound( ShapeId const id ) const
//{
//	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
//	return p ? p->HasTriggerSound( ) : false; 
//}
//
//Hertz const NNetModelReaderInterface::GetTriggerSoundFrequency( ShapeId const id ) const
//{
//	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
//	return p ? p->GetTriggerSoundFrequency( ) : Hertz::NULL_VAL(); 
//}
//
//MilliSecs const NNetModelReaderInterface::GetTriggerSoundDuration ( ShapeId const id ) const
//{
//	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) };
//	return p ? p->GetTriggerSoundDuration( ) : MilliSecs::NULL_VAL(); 
//}
//
mV const NNetModelReaderInterface::GetVoltage( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->GetVoltage( ) : mV::NULL_VAL(); 
}

mV const NNetModelReaderInterface::GetVoltage( ShapeId const id, MicroMeterPoint const & umPoint ) const
{
	auto p { m_pModel->GetShapeConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltage( umPoint ) : mV::NULL_VAL(); 
}

size_t const NNetModelReaderInterface::GetNrOfOutgoingConnections( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.GetNrOfOutgoingConnections( ) : -1;
}

size_t const NNetModelReaderInterface::GetNrOfIncomingConnections( ShapeId const id ) const 
{ 
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.GetNrOfIncomingConnections( ) : -1;
}

bool const NNetModelReaderInterface::HasIncoming( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.HasIncoming( ) : false; 
}

bool const NNetModelReaderInterface::HasOutgoing( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.HasOutgoing( ) : false; 
}

MicroMeterRect const NNetModelReaderInterface::GetEnclosingRect( ) const
{
	return m_pModel->GetEnclosingRect( );
}

fMicroSecs NNetModelReaderInterface::GetSimulationTime( ) const 
{ 
	return m_pModel->GetSimulationTime( ); 
}

bool const NNetModelReaderInterface::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( idSrc == idDst )
		return false;

	if ( (idSrc == NO_SHAPE) || (idDst == NO_SHAPE) )
		return false;

	if ( isConnectedTo( idSrc, idDst ) )  // if already connected we cannot connect again
		return false;

	ShapeType const typeSrc { GetShapeType( idSrc ) };
	ShapeType const typeDst { GetShapeType( idDst ) };

	switch ( typeSrc.GetValue( ) )
	{
	case ShapeType::Value::pipe:
		return false;

	case ShapeType::Value::knot:
		switch ( typeSrc.GetValue( ) )
		{
		case ShapeType::Value::pipe:
		case ShapeType::Value::knot:
			return true;

		case ShapeType::Value::neuron:
			return (GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) <= 1);

		case ShapeType::Value::inputNeuron:
			return false;

		default:
			break;
		}
		break;

	case ShapeType::Value::neuron:
		switch ( typeSrc.GetValue( ) )
		{
		case ShapeType::Value::pipe:
			return true;

		case ShapeType::Value::neuron:
			return (GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) <= 1);

		case ShapeType::Value::knot:
		case ShapeType::Value::inputNeuron:
			return false;

		default:
			break;
		}
		break;

	case ShapeType::Value::inputNeuron:
		return ( typeDst.IsAnyNeuronType() )
	           ? (GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) <= 1) && ! HasIncoming( idSrc )
			   : false;
		break;

	default:
		break;
	}

	return false;
}

bool const NNetModelReaderInterface::isConnectedTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( GetShapeType( idSrc ).IsPipeType() )
		return isConnectedToPipe( idDst, idSrc );
	if ( GetShapeType( idDst ).IsPipeType() )
		return isConnectedToPipe( idSrc, idDst );
	else
		return false;
}

bool const NNetModelReaderInterface::isConnectedToPipe( ShapeId const idShape, ShapeId const idPipe ) const
{
	return (idShape == m_pModel->GetStartKnotId(idPipe)) || (idShape == m_pModel->GetEndKnotId(idPipe));
}

ShapeId const NNetModelReaderInterface::FindShapeAt( MicroMeterPoint const & umPnt, ShapeCrit const & crit ) const
{
	return m_pModel->FindShapeAt( umPnt, crit );
}

unsigned long const NNetModelReaderInterface::GetNrOfPipes       ( ) const { return Pipe       ::GetCounter(); }
unsigned long const NNetModelReaderInterface::GetNrOfShapes      ( ) const { return Shape      ::GetCounter(); }
unsigned long const NNetModelReaderInterface::GetNrOfKnots       ( ) const { return Knot       ::GetCounter(); }
unsigned long const NNetModelReaderInterface::GetNrOfInputNeurons( ) const { return InputNeuron::GetCounter(); }
unsigned long const NNetModelReaderInterface::GetNrOfNeurons     ( ) const { return Neuron     ::GetCounter() - GetNrOfInputNeurons(); }

void NNetModelReaderInterface::DrawExterior
( 
	ShapeId        const   id,
	DrawContext    const & context,
	tHighlightType const   highlightType
) const
{
	if ( Shape const * const p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawExterior( context, highlightType );
}

void NNetModelReaderInterface::DrawInterior
( 
	ShapeId     const   id, 
	DrawContext const & context
) 
	const
{
	if ( auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawInterior( context );
}
