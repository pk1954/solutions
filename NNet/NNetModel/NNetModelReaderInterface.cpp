// NNetModelReaderInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Shape.h"
#include "NNetModel.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModelReaderInterface.h"

bool const NNetModelReaderInterface::IsSelected( ShapeId const id ) const
{
	auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) };
	return p ? p->IsSelected( ) : false; 
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
	auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) }; 
	return p ? p->GetTriggerSound( ) : SoundDescr(); 
}

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

bool const NNetModelReaderInterface::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( idSrc == idDst )
		return false;

	if ( IsUndefined(idSrc) || IsUndefined(idDst) )
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
		switch ( typeDst.GetValue( ) )
		{
		case ShapeType::Value::pipe:
		case ShapeType::Value::knot:
			return true;

		case ShapeType::Value::neuron:
			return (GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) <= 1);

		case ShapeType::Value::inputNeuron:
			return (GetNrOfOutgoingConnections( idDst ) == 0);

		default:
			break;
		}
		break;

	case ShapeType::Value::neuron:
		switch ( typeDst.GetValue( ) )
		{
		case ShapeType::Value::pipe:
			return true;

		case ShapeType::Value::knot:
		case ShapeType::Value::neuron:
			return (GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) <= 1);

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

MicroMeterPoint const NNetModelReaderInterface::OrthoVector( ShapeId const idPipe ) const
{
	MicroMeterPoint upVector { m_pModel->GetShapeConstPtr<Pipe const *>( idPipe )->GetVector() };
	return upVector.OrthoVector( NEURON_RADIUS * 2.f );
}

void NNetModelReaderInterface::DrawExterior
( 
	ShapeId        const   id,
	DrawContext    const & context,
	tHighlightType const   type
) const
{
	if ( Shape const * const p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawExterior( context, type );
}

void NNetModelReaderInterface::DrawInterior
( 
	ShapeId        const   id, 
	DrawContext    const & context,
	tHighlightType const   type
) 
const
{
	if ( auto p { m_pModel->GetShapeConstPtr<Shape const *>(id) } )
		p->DrawInterior( context, type );
}

void NNetModelReaderInterface::DrawNeuronText
( 
	ShapeId     const   id, 
	DrawContext const & context
) 
const
{
	if ( auto p { m_pModel->GetShapeConstPtr<Neuron const *>(id) } )
		p->DrawNeuronText( context );
}
