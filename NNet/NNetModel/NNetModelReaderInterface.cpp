// NNetModelReaderInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "DrawContext.h"
#include "NNetModel.h"
#include "Connector.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModelReaderInterface.h"

bool const NNetModelReaderInterface::IsSelected( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->IsSelected() : false; 
}

NobType const NNetModelReaderInterface::GetNobType( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<Nob const *>(id) };
	return p ? p->GetNobType() : NobType::Value::undefined; 
}

Degrees const NNetModelReaderInterface::GetDirection( NobId const id ) const 
{ 
	auto p { m_pModel->GetNobConstPtr<Connector const *>(id) };
	return p ? Radian2Degrees(p->GetDir()) : Degrees::NULL_VAL(); 
}

fHertz const NNetModelReaderInterface::GetPulseFrequency( NobId const id ) const 
{ 
	auto p { m_pModel->GetNobConstPtr<InputNeuron const *>(id) };
	return p ? p->GetPulseFrequency() : fHertz::NULL_VAL(); 
}

size_t const NNetModelReaderInterface::GetNrOfSegments( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetNrOfSegments() : 0; 
}

SoundDescr const NNetModelReaderInterface::GetTriggerSound( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<Neuron const *>(id) }; 
	return p ? p->GetTriggerSound() : SoundDescr(); 
}

mV const NNetModelReaderInterface::GetVoltage( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->GetVoltage() : mV::NULL_VAL(); 
}

mV const NNetModelReaderInterface::GetVoltage( NobId const id, MicroMeterPoint const & umPoint ) const
{
	auto p { m_pModel->GetNobConstPtr<Pipe const *>(id) };
	return p ? p->GetVoltage( umPoint ) : mV::NULL_VAL(); 
}

size_t const NNetModelReaderInterface::GetNrOfOutgoingConnections( NobId const id ) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.GetNrOfOutgoingConnections() : -1;
}

size_t const NNetModelReaderInterface::GetNrOfIncomingConnections( NobId const id ) const 
{ 
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.GetNrOfIncomingConnections() : -1;
}

bool const NNetModelReaderInterface::HasIncoming( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.HasIncoming() : false; 
}

bool const NNetModelReaderInterface::HasOutgoing( NobId const id ) const
{
	auto p { m_pModel->GetNobConstPtr<BaseKnot const *>(id) };
	return p ? p->m_connections.HasOutgoing() : false; 
}

bool const NNetModelReaderInterface::GetDescriptionLine( int const iLine, wstring & wstrLine ) const 
{
	return m_pModel->GetDescriptionLine( iLine, wstrLine );
};

// IsConnectionCandidate: Sort out obvious non-candidates

bool const NNetModelReaderInterface::IsConnectionCandidate(NobId const idSrc, NobId const idDst) const
{
	if (idSrc == idDst)
		return false; 
	if (IsConnectedTo(idSrc, idDst)) // if already connected we cannot connect again
		return false;
	NobType::Value const typeSrc { GetNobType(idSrc).GetValue() };
	NobType::Value const typeDst { GetNobType(idDst).GetValue() };
	if ( (typeSrc == NobType::Value::connector) != (typeDst == NobType::Value::connector) )
		return false;
	if ( (typeSrc == NobType::Value::closedConnector) || (typeDst == NobType::Value::closedConnector) )
		return false;
	return true;
}

bool const NNetModelReaderInterface::CanConnectTo(NobId const idSrc, NobId const idDst) const
{
	assert(idSrc != idDst);
	assert(IsDefined(idSrc));
	assert(IsDefined(idDst));
	assert(!IsConnectedTo(idSrc, idDst));

	NobType::Value const typeSrc { GetNobType(idSrc).GetValue() };
	NobType::Value const typeDst { GetNobType(idDst).GetValue() };

	switch (typeSrc)
	{
	case NobType::Value::connector:
		if (typeDst == NobType::Value::connector)
		{
			Connector const & connSrc { * m_pModel->GetNobConstPtr<Connector const *>(idSrc) }; 
			Connector const & connDst { * m_pModel->GetNobConstPtr<Connector const *>(idDst) }; 
			if (connSrc.IsInputConnector() == connDst.IsInputConnector())  // opposite connectors?
				return false;
			if (connSrc.Size() == connDst.Size())
				return true;
		}
		return false;

	case NobType::Value::pipe:
		return false;

	case NobType::Value::knot:
		switch (typeDst)
		{
		case NobType::Value::pipe:
		case NobType::Value::knot:
			return true;

		case NobType::Value::outputNeuron:
			return (! HasOutgoing(idSrc));

		case NobType::Value::inputNeuron:
		case NobType::Value::neuron:
			return onlyOneAxon(idSrc, idDst);

		default:
			break;
		}
		break;

	case NobType::Value::neuron:
		switch ( typeDst )
		{
		case NobType::Value::pipe:
			return true;

		case NobType::Value::knot:
		case NobType::Value::neuron:
			return onlyOneAxon(idSrc, idDst);

		case NobType::Value::outputNeuron:
			return (! HasOutgoing(idSrc));

		default:
			break;
		}
		break;

	case NobType::Value::inputNeuron:
		switch ( typeDst )
		{
		case NobType::Value::neuron:
		case NobType::Value::inputNeuron:
			return onlyOneAxon(idSrc, idDst) && ! HasIncoming(idSrc);

		case NobType::Value::outputNeuron:
			return true;

		default:
			break;
		}
		break;

	case NobType::Value::outputNeuron:
		switch ( typeDst )
		{
		case NobType::Value::outputNeuron:
		case NobType::Value::inputNeuron:
			return true;

		case NobType::Value::knot:
		case NobType::Value::neuron:
			return onlyOneAxon(idSrc, idDst);

		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}

bool const NNetModelReaderInterface::IsConnectedTo( NobId const idSrc, NobId const idDst ) const
{
	if ( GetNobType(idSrc).IsPipeType() )
		return isConnectedToPipe(idDst, idSrc);
	if ( GetNobType(idDst).IsPipeType() )
		return isConnectedToPipe(idSrc, idDst );
	else
		return false;
}

bool const NNetModelReaderInterface::isConnectedToPipe( NobId const idNob, NobId const idPipe ) const
{
	return (idNob == m_pModel->GetStartKnotId(idPipe)) || (idNob == m_pModel->GetEndKnotId(idPipe));
}

NobId const NNetModelReaderInterface::FindNobAt( MicroMeterPoint const & umPnt, NobCrit const & crit ) const
{
	return m_pModel->FindNobAt( umPnt, crit );
}

void NNetModelReaderInterface::DrawExterior
( 
	NobId     const   id,
	DrawContext const & context,
	tHighlight  const   type
) const
{
	if ( Nob const * const p { m_pModel->GetNobConstPtr<Nob const *>(id) } )
		p->DrawExterior( context, type );
}

void NNetModelReaderInterface::DrawInterior
( 
	NobId     const   id, 
	DrawContext const & context,
	tHighlight  const   type
) 
const
{
	if ( auto p { m_pModel->GetNobConstPtr<Nob const *>(id) } )
		p->DrawInterior( context, type );
}

void NNetModelReaderInterface::DrawNeuronText
( 
	NobId     const   id, 
	DrawContext const & context
) 
const
{
	if ( auto p { m_pModel->GetNobConstPtr<Neuron const *>(id) } )
		p->DrawNeuronText( context );
}

void NNetModelReaderInterface::DrawLine
( 
	MicroMeterLine const & umLine, 
	DrawContext    const & context 
) const
{
	context.DrawLine( umLine.GetStartPoint(), umLine.GetEndPoint(), 10.0_MicroMeter, D2D1::ColorF::Brown );
}
