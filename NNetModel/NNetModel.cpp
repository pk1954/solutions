// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModel.h"

#ifndef NDEBUG
	#define CHECK_CONSISTENCY checkConsistency( )
#else
	#define CHECK_CONSISTENCY
#endif

using namespace std::chrono;
using std::unordered_map;

wchar_t const * const NNetModel::GetParameterName( tParameter const p ) const
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,        L"PulseRate"        },
		{ tParameter::pulseSpeed,       L"PulseSpeed"       },
		{ tParameter::pulseWidth,       L"PulseWidth"       },
		{ tParameter::signalLoss,       L"SignalLoss"       },
		{ tParameter::threshold,        L"Threshold"        },
		{ tParameter::peakVoltage,      L"PeakVoltage"      },
		{ tParameter::refractoryPeriod, L"RefractoryPeriod" }
	};				  

	return mapParam.at( p );
}

wchar_t const * const NNetModel::GetParameterUnit( tParameter const p ) const
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,        L"Hz"    },
		{ tParameter::pulseSpeed,       L"m/sec" },
		{ tParameter::pulseWidth,       L"µs"    },
		{ tParameter::signalLoss,       L"1/µm"  },
		{ tParameter::threshold,        L"mV"    },
		{ tParameter::peakVoltage,      L"mV"    },
		{ tParameter::refractoryPeriod, L"µs"    }
	};				  

	return mapParam.at( p );
}

NNetModel::NNetModel( )
  : m_Shapes( ),
	m_timeStamp            ( 0._MicroSecs ),
	m_signalLoss           ( 0.0005f ), 
	m_threshold            ( 20._mV ),
	m_peakVoltage          ( 10._mV ),
	m_pulseWidth           ( 2000._MicroSecs ),
	m_refractoryPeriod     ( 500._MicroSecs ),
	m_pulseSpeed           ( 0.1_meterPerSec )
{					
	// initial shapes 
	ShapeId m_idInputNeuron = NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
	ShapeId m_idNeuron      = NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) );
	ShapeId m_idPipeline    = NewPipeline( m_idInputNeuron, m_idNeuron );
}

NNetModel::~NNetModel( )
{}

void NNetModel::RecalcPipelines( ) 
{ 
	Apply2All<Pipeline>( [&]( Pipeline & pipe ) { pipe.Recalc( ); } );
} 

long const NNetModel::GetNrOfShapes( ) const
{
	long lCounter = 0;
	Apply2All<Shape>( [&]( Shape & shape ) { ++ lCounter; } );
	return lCounter;
}

bool NNetModel::areConnected( ShapeId const id1, ShapeId const id2 )
{
	bool bConnectionFound { false };

	GetTypedShape<BaseKnot>( id1 )->Apply2AllIncomingPipelines
	( 
		[&]( ShapeId & idPipeline ) 
		{
			if ( GetTypedShape<Pipeline>( idPipeline )->GetStartKnot() == id2 ) 
				bConnectionFound = true; 
		} 
	);

	if ( bConnectionFound )
		return true;

	GetTypedShape<BaseKnot>( id2 )->Apply2AllIncomingPipelines
	( 
		[&]( ShapeId & idPipeline ) 
		{
			if ( GetTypedShape<Pipeline>( idPipeline )->GetStartKnot() == id1 ) 
				bConnectionFound = true; 
		} 
	);

	return bConnectionFound;
}

bool const NNetModel::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	tShapeType const typeSrc { GetShapeType( idSrc ) };
	tShapeType const typeDst { GetShapeType( idDst ) };

	if ( idSrc == idDst )
		return false;

	if ( ! IsBaseKnotType( typeSrc ) )
		return false;

	if ( ! IsBaseKnotType( typeDst ) )
		return false;

	BaseKnot const & baseKnotSrc { * GetConstTypedShape<BaseKnot>( idSrc ) };
	BaseKnot const & baseKnotDst { * GetConstTypedShape<BaseKnot>( idDst ) };

	if ( (typeDst == tShapeType::inputNeuron) && baseKnotSrc.HasIncoming() )  // cannot connect incoming dendrite
		return false;                                                         // to input neuron

	size_t nrOfOutgoingConnections { baseKnotSrc.GetNrOfOutgoingConnections() + baseKnotDst.GetNrOfOutgoingConnections() };

	if ( IsNeuronType( typeDst ) && ( nrOfOutgoingConnections > 1 ) ) // neurons do not have 
		return false;                                                 // more than one axon

	return true;
}

void NNetModel::RemoveShape( ShapeId const idShapeToBeDeleted )
{
	switch ( GetShapeType( idShapeToBeDeleted ) )
	{
	case tShapeType::pipeline:
		deletePipeline( idShapeToBeDeleted );
		break;

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
		deleteBaseKnot( idShapeToBeDeleted );
		break;

	case tShapeType::knot:
	default:
		break;
	}
	CHECK_CONSISTENCY;
}

void NNetModel::deleteBaseKnot( ShapeId const id )
{
	BaseKnot * pToBeDeleted { GetTypedShape<BaseKnot>( id ) };
	if ( pToBeDeleted )
	{
		MicroMeterPoint umPos { pToBeDeleted->GetPosition() };
		assert( ::IsBaseKnotType( pToBeDeleted->GetShapeType( ) ) );
		m_Shapes[ id.GetValue() ] = nullptr;
		pToBeDeleted->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const idPipeline ) // every incoming pipeline needs a new end knot
			{ 
				Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
				Knot     * pKnotNew  { new Knot( this, umPos ) };
				pPipeline->SetEndKnot( addShape( pKnotNew ) );
				pKnotNew->AddIncomming( idPipeline );
			} 
		);
		pToBeDeleted->Apply2AllOutgoingPipelines
		( 
			[&]( ShapeId const idPipeline ) // every outgoing pipeline needs a new start knot
			{ 
				Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
				Knot     * pKnotNew  { new Knot( this, umPos ) };
				pPipeline->SetStartKnot( addShape( pKnotNew ) );
				pKnotNew->AddOutgoing( idPipeline );
			} 
		);
		delete pToBeDeleted;
	}
}

void NNetModel::deletePipeline( ShapeId const id )
{
	Pipeline * pPipelineToBeDeleted { GetTypedShape<Pipeline>( id ) };
	if ( pPipelineToBeDeleted )
	{
		assert( ::IsPipelineType( pPipelineToBeDeleted->GetShapeType( ) ) );
		m_Shapes[ id.GetValue() ] = nullptr;

		ShapeId    idStartKnot { pPipelineToBeDeleted->GetStartKnot() };
		BaseKnot * pStartKnot  { GetTypedShape<BaseKnot>( idStartKnot ) };
		if ( pStartKnot )
		{
			pStartKnot->RemoveOutgoing( id );
			if ( pStartKnot->IsOrphanedKnot( ) )
				deleteBaseKnot( idStartKnot );
		}

		ShapeId    idEndKnot { pPipelineToBeDeleted->GetEndKnot() };
		BaseKnot * pEndKnot  { GetTypedShape<BaseKnot>( idEndKnot ) };
		if ( pEndKnot )
		{
			pEndKnot->RemoveIncoming( id );
			if ( pEndKnot->IsOrphanedKnot( ) )
				deleteBaseKnot( idEndKnot );
		}

		delete pPipelineToBeDeleted;
	}
}

float const NNetModel::GetPulseRate( Shape const * pShape ) const
{
	assert( pShape );
	return Cast2InputNeuron( pShape )->GetPulseFrequency().GetValue();
}

float const NNetModel::GetParameterValue( tParameter const param ) const
{
	switch ( param )
	{
	case tParameter::pulseSpeed:
		return m_pulseSpeed.GetValue();

	case tParameter::pulseWidth:
		return m_pulseWidth.GetValue();

	case tParameter::signalLoss:
		return m_signalLoss;

	case tParameter::threshold:
		return m_threshold.GetValue();

	case tParameter::peakVoltage:
		return m_peakVoltage.GetValue();

	case tParameter::refractoryPeriod:
		return m_refractoryPeriod.GetValue();

	default  :
		assert( false );
	}
	return 0.f;
}

void NNetModel::SetPulseRate( ShapeId const id, float const fNewValue )
{
	InputNeuron * const pInputNeuron { GetTypedShape<InputNeuron>( id ) };
	if ( pInputNeuron )
		pInputNeuron->SetPulseFrequency( static_cast< fHertz >( fNewValue ) );
}

void NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	switch ( param )
	{
	case tParameter::pulseSpeed:
		m_pulseSpeed = static_cast< meterPerSec >( fNewValue );
		RecalcPipelines( );
		break;

	case tParameter::pulseWidth:
		m_pulseWidth = static_cast< MicroSecs >( fNewValue );
		break;

	case tParameter::signalLoss:
		m_signalLoss = fNewValue;
		RecalcPipelines( );
		break;

	case tParameter::threshold:
		m_threshold = static_cast< mV >( fNewValue );
		break;

	case tParameter::peakVoltage:
		m_peakVoltage = static_cast< mV >( fNewValue );
		break;

	case tParameter::refractoryPeriod:
		m_refractoryPeriod = static_cast< MicroSecs >( fNewValue );
		break;

	default  :
		assert( false );
	}
}

MicroMeterPoint const NNetModel::GetShapePos( ShapeId const id ) const 
{
	if ( BaseKnot const * pBaseKnot = GetConstTypedShape<BaseKnot>( id ) )
		return pBaseKnot->GetPosition();
	return NP_NULL;
}

tShapeType const NNetModel::GetShapeType( ShapeId const id ) const 
{
	if ( Shape const * pShape = GetConstShape( id ) )
		return pShape->GetShapeType();
	return tShapeType::undefined;
}

bool const NNetModel::HasOutgoing( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetConstTypedShape<BaseKnot>( id ) )
		return pBaseKnot->HasOutgoing( );
	return false;
}

bool const NNetModel::HasIncoming( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetConstTypedShape<BaseKnot>( id ) )
		return pBaseKnot->HasIncoming( );
	return false;
}

size_t const NNetModel::GetNrOfOutgoingConnections( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetConstTypedShape<BaseKnot>( id ) )
		return pBaseKnot->GetNrOfOutgoingConnections( );
	return false;
}

size_t const NNetModel::GetNrOfIncomingConnections( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetConstTypedShape<BaseKnot>( id ) )
		return pBaseKnot->GetNrOfIncomingConnections( );
	return false;
}

void NNetModel::Connect( ShapeId const idSrc, ShapeId const idDst )  // merge src shape into dst shape
{
	BaseKnot * pSrc = GetTypedShape<BaseKnot>( idSrc );
	BaseKnot * pDst = GetTypedShape<BaseKnot>( idDst );
	if ( pSrc && pDst )
	{
		pSrc->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				pDst->AddIncomming( idPipeline );
				GetTypedShape<Pipeline>(idPipeline)->SetEndKnot( idDst );
			}
		);

		pSrc->Apply2AllOutgoingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				pDst->AddOutgoing( idPipeline );
				GetTypedShape<Pipeline>(idPipeline)->SetStartKnot( idDst );
			}
		);
		m_Shapes[ idSrc.GetValue() ] = nullptr;
		delete pSrc;
	}
	CHECK_CONSISTENCY;
}

ShapeId const NNetModel::addShape( Shape * pShape )
{
	ShapeId id( CastToLong( m_Shapes.size() ) );  
	m_Shapes.push_back( pShape );                 
	pShape->SetId( id );
	return id;
}

void NNetModel::ConnectPipeline
(
	Pipeline *    pPipeline,
	ShapeId const idPipeline,
	ShapeId const idStart, 
	ShapeId const idEnd 
)
{
	pPipeline->SetStartKnot( idStart );
	pPipeline->SetEndKnot  ( idEnd );
	GetTypedShape<BaseKnot>( idStart )->AddOutgoing ( idPipeline );
	GetTypedShape<BaseKnot>( idEnd   )->AddIncomming( idPipeline );
}

ShapeId NNetModel::NewPipeline( ShapeId const idStart, ShapeId const idEnd )
{
	Pipeline * pPipelineNew { new Pipeline( this ) };
	ShapeId const id { addShape( pPipelineNew ) };
	ConnectPipeline( pPipelineNew, id, idStart, idEnd );
	return id;
}

void NNetModel::insertNewBaseKnot( ShapeId const id, BaseKnot * const pNewBaseKnot )
{
	Pipeline * const pPipeline { GetTypedShape<Pipeline>( id )};
	if ( pPipeline )
	{
		ShapeId const idStartKnot{ pPipeline->GetStartKnot() };
		ShapeId const idNewShape { addShape( pNewBaseKnot ) };
		NewPipeline( idStartKnot, idNewShape );
		GetTypedShape<BaseKnot>( idStartKnot )->RemoveOutgoing( id );
		GetTypedShape<BaseKnot>( idNewShape  )->AddOutgoing   ( id );
		pPipeline->SetStartKnot( idNewShape );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	switch ( GetShapeType( id ) )
	{
	case tShapeType::pipeline:
	{
		Pipeline * const pPipelineHighlighted { GetTypedShape<Pipeline>( id )};
		GetTypedShape<BaseKnot>( pPipelineHighlighted->GetStartKnot() )->MoveShape( delta );
		GetTypedShape<BaseKnot>( pPipelineHighlighted->GetEndKnot  () )->MoveShape( delta );
		break;
	}

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
		GetTypedShape<BaseKnot>( id )->MoveShape( delta );
		break;

	default:
		assert( false );
	}
}

ShapeId const NNetModel::splitPipeline( ShapeId const id, MicroMeterPoint const & splitPoint )
{
	Knot * pKnotNew { new Knot( this, splitPoint ) };
	insertNewBaseKnot( id, pKnotNew );
	return pKnotNew->GetId( );
}

ShapeId const NNetModel::InsertNeuron( ShapeId const id, MicroMeterPoint const & splitPoint )
{
	Neuron * pNeuronNew { new Neuron( this, splitPoint ) };
	insertNewBaseKnot( id, pNeuronNew );
	return pNeuronNew->GetId( );
}

MicroMeterPoint NNetModel::orthoVector( ShapeId const idPipeline ) const
{
	Pipeline const * pPipeline { GetConstTypedShape<Pipeline>( idPipeline ) };
	return OrthoVector( pPipeline->GetVector(), NEURON_RADIUS * 2.f );
}

void NNetModel::AddOutgoing( ShapeId const id, MicroMeterPoint const & pos )
{
	switch ( GetShapeType( id ) )
	{
	case tShapeType::pipeline:
		NewPipeline( splitPipeline( id, pos ), NewShape<Knot>( pos + orthoVector( id ) ) );
		break;

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
		NewPipeline( id, NewShape<Knot>( pos + STD_OFFSET) );
		break;

	default:
		break;
	}
	CHECK_CONSISTENCY;
}

void NNetModel::AddIncoming( ShapeId const id, MicroMeterPoint const & pos )
{
	switch ( GetShapeType( id ) )
	{
	case tShapeType::pipeline:
		NewPipeline( NewShape<Knot>( pos - orthoVector( id ) ), splitPipeline( id, pos ) );
		break;

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
		NewPipeline( NewShape<Knot>( pos - STD_OFFSET ), id );
		break;

	default:
		break;
	}
	CHECK_CONSISTENCY;
}

void NNetModel::CheckConsistency( Shape const * pShape ) const
{
	tShapeType type = pShape->GetShapeType();

	if ( ::IsBaseKnotType( type ) )
		static_cast<BaseKnot const &>( * pShape ).Apply2AllConnectedPipelinesConst
		( 
			[&]( ShapeId const id ) { assert( IsType<Pipeline>( id ) ); } 
	    );

	switch ( type )
	{
	case tShapeType::inputNeuron:
		assert( ! static_cast<InputNeuron const *>( pShape )->HasIncoming() );
		break;

	case tShapeType::knot:
		break;

	case tShapeType::neuron:
		break;

	case tShapeType::pipeline:
		{
			Pipeline const * pPipeline { static_cast<Pipeline const *>( pShape ) };
			assert( pPipeline );
			Shape const * const pStart { GetConstShape( pPipeline->GetStartKnot() ) };
			Shape const * const pEnd   { GetConstShape( pPipeline->GetEndKnot() ) };
			if ( pStart )
				assert( StartKnotType::TypeFits( pStart->GetShapeType() ) );
			if ( pEnd )
				assert( EndKnotType::TypeFits( pEnd->GetShapeType() ) );
			break;
		}

	case tShapeType::undefined:
		assert( false );
		break;

	default:
		assert( false );
	}
}

void NNetModel::checkConsistency( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { CheckConsistency( & shape ); } );
}

void NNetModel::Apply2GlobalParameters( function<void(tParameter const &)> const & func ) const
{
	for ( int i = 0; i <= static_cast<int>(tParameter::tParameterLastGlobal); ++i )
		func( static_cast<tParameter>( i ) );
}

void NNetModel::Apply2AllParameters( function<void(tParameter const &)> const & func ) const
{
	for ( int i = 0; i <= static_cast<int>(tParameter::tParameterLast); ++i )
		func( static_cast<tParameter>( i ) );
}

void NNetModel::CopyModelData( ModelInterface const * const src )
{
	* this = * static_cast<NNetModel const *>( src );
}

void NNetModel::Compute( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Step   ( ); } );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { delete & shape; } );
	m_Shapes.clear();
}

Shape const * NNetModel::FindShapeAt( MicroMeterPoint const pnt, function<bool(Shape const &)> const & crit ) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape;
	};

	return nullptr;
}

Shape const * NNetModel::FindShapeAt( MicroMeterPoint const pnt ) const
{
	
	Shape const * pRes { nullptr };

	if ( ! pRes )   // first test all knot shapes
		pRes = FindShapeAt( pnt, [&]( Shape const & s ) { return ::IsKnotType( s.GetShapeType() ); } ); 	

	if ( ! pRes )  // if nothing found, test neurons and input neurons
		pRes = FindShapeAt( pnt, [&]( Shape const & s ) { return ::IsNeuronType( s.GetShapeType() ); } );

	if ( ! pRes ) // if nothing found, try pipelines
		pRes = FindShapeAt( pnt, [&]( Shape const & s ) { return ::IsPipelineType( s.GetShapeType() ); } );

	return pRes;
}

COLORREF const NNetModel::GetFrameColor( tHighlightType const type ) const 
{ 
	return ( type == tHighlightType::superHighlighted )
		   ? EXT_COLOR_SUPER_HIGHLIGHT 
		   : ( type == tHighlightType::highlighted )
			  ? EXT_COLOR_HIGHLIGHT 
			  : EXT_COLOR_NORMAL;
};
