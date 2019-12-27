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
	#define CHECK_CONSISTENCY Apply2All<Shape>( [&]( Shape & shape ) { checkConsistency( & shape ); } )
#else
	#define CHECK_CONSISTENCY
#endif

using namespace std::chrono;
using std::unordered_map;

CRITICAL_SECTION NNetModel::m_criticalSection;
bool             NNetModel::m_bCritSectReady = false;

NNetModel::NNetModel( )
  : m_Shapes( ),
	m_timeStamp       ( 0._MicroSecs ),
	m_signalLoss      ( 0.0005f ), 
	m_threshold       ( 20._mV ),
	m_peakVoltage     ( 10._mV ),
	m_pulseWidth      ( 2000._MicroSecs ),
	m_refractPeriod( 500._MicroSecs ),
	m_pulseSpeed      ( 0.1_meterPerSec ),
	m_bUnsavedChanges ( false )
{					
	if ( ! m_bCritSectReady )
	{
		(void)InitializeCriticalSectionAndSpinCount( & m_criticalSection, 0x00000400 );
		m_bCritSectReady = true;
	}

	createInitialShapes( );
}

NNetModel::~NNetModel( )
{
	if ( m_bCritSectReady )
	{
		DeleteCriticalSection( & m_criticalSection );
		m_bCritSectReady = false;
	}
}

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

bool const NNetModel::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( idSrc == idDst )
		return false;

	if ( IsConnectedTo( idSrc, idDst ) )  // if already connected we cannot connect again
		return false;

	tShapeType const typeSrc { GetShapeType( idSrc ) };

	if ( IsPipelineType( typeSrc ) )          // only BaseKnots can be connected
		return false;                         // to other shapes

	if ( typeSrc == tShapeType::inputNeuron ) // input neuron cannot connect to anything
		return false;                         // try the other way round

	tShapeType const typeDst { GetShapeType( idDst ) };

	if ( IsPipelineType( typeDst ) )
	{
		if ( typeSrc == tShapeType::inputNeuron )   // cannot insert input neuron
			return false;                           // into pipeline
	}
	else if ( IsAnyNeuronType( typeDst ) )
	{
		if ( IsAnyNeuronType( typeSrc ) )  // neurons cannot connect directly 
			return false;               // to other neurons

		if ( (typeDst == tShapeType::inputNeuron) && HasIncoming( idSrc ) )  // cannot connect incoming dendrite
			return false;                                                    // to input neuron

		if ( GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) > 1 ) // neurons can not not have 
			return false;                                                                    // more than one axon
	}

	return true;
}

void NNetModel::RemoveShape( ShapeId const idShapeToBeDeleted )
{
	m_bUnsavedChanges = true;
	switch ( GetShapeType( idShapeToBeDeleted ) )
	{
	case tShapeType::pipeline:
		deletePipeline( idShapeToBeDeleted );
		break;

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
		Disconnect( idShapeToBeDeleted );
		deleteShape( idShapeToBeDeleted );
		break;

	case tShapeType::knot:
	default:
		break;
	}
	CHECK_CONSISTENCY;
}

void NNetModel::Disconnect( ShapeId const id )
{
	if ( BaseKnot * pBaseKnot { GetTypedShape<BaseKnot>( id ) } )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		
		pBaseKnot->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const idPipeline ) 
			{ 
				Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
				Knot     * pKnotNew  { new Knot( this, umPos ) };
				pPipeline->SetEndKnot( addShape( pKnotNew ) );
				pKnotNew->AddIncomming( idPipeline );
				pBaseKnot->RemoveIncoming( idPipeline );
				pPipeline->DislocateEndPoint( );
			} 
		);
		
		pBaseKnot->Apply2AllOutgoingPipelines
		( 
			[&]( ShapeId const idPipeline ) 
			{ 
				Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
				Knot     * pKnotNew  { new Knot( this, umPos ) };
				pPipeline->SetStartKnot( addShape( pKnotNew ) );
				pKnotNew->AddOutgoing( idPipeline );
				pBaseKnot->RemoveOutgoing( idPipeline );
				pPipeline->DislocateStartPoint( );
			} 
		);

		assert( pBaseKnot->IsOrphan( ) );

		if ( pBaseKnot->GetShapeType( ) == tShapeType::knot )
			deleteShape( id );
	}
	CHECK_CONSISTENCY;
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
		case tParameter::pulseSpeed:	   return m_pulseSpeed.GetValue();
		case tParameter::pulseWidth:	   return m_pulseWidth.GetValue();
		case tParameter::signalLoss:	   return m_signalLoss;
		case tParameter::threshold:  	   return m_threshold.GetValue();
		case tParameter::peakVoltage:	   return m_peakVoltage.GetValue();
		case tParameter::refractPeriod: return m_refractPeriod.GetValue();
		default: assert( false );
	}
	return 0.f;
}

void NNetModel::SetPulseRate( ShapeId const id, float const fNewValue )
{
	m_bUnsavedChanges = true;
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
	m_bUnsavedChanges = true;
	switch ( param )
	{
		case tParameter::pulseSpeed:	m_pulseSpeed    = static_cast< meterPerSec >( fNewValue ); break;
		case tParameter::pulseWidth:	m_pulseWidth    = static_cast< MicroSecs >  ( fNewValue ); break;
		case tParameter::signalLoss:	m_signalLoss    =                           ( fNewValue ); break;
		case tParameter::threshold:		m_threshold     = static_cast< mV >         ( fNewValue ); break;
		case tParameter::peakVoltage:	m_peakVoltage   = static_cast< mV >         ( fNewValue ); break;
		case tParameter::refractPeriod:	m_refractPeriod = static_cast< MicroSecs >  ( fNewValue ); break;
		default: assert( false );
	}
	RecalcPipelines( );
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
	if ( IsPipelineType( GetShapeType( idDst ) ) )   // connect baseknot to pipeline
	{
		insertBaseKnot( idDst, idSrc );
	}
	else  // connect baseknot to baseknot
	{
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

			if ( pSrc->GetShapeType() == tShapeType::knot )
				deleteShape( idSrc );
		}
	}
	m_bUnsavedChanges = true;
	CHECK_CONSISTENCY;
}

void NNetModel::ConnectPipeline
(
	Pipeline *    pPipeline,
	ShapeId const idPipeline,
	ShapeId const idStart, 
	ShapeId const idEnd 
)
{
	m_bUnsavedChanges = true;
	pPipeline->SetStartKnot( idStart );
	pPipeline->SetEndKnot  ( idEnd   );
	GetTypedShape<BaseKnot>( idStart )->AddOutgoing ( idPipeline );
	GetTypedShape<BaseKnot>( idEnd   )->AddIncomming( idPipeline );
	CHECK_CONSISTENCY;
}

ShapeId NNetModel::NewPipeline( ShapeId const idStart, ShapeId const idEnd )
{
	m_bUnsavedChanges = true;
	Pipeline * pPipelineNew { new Pipeline( this ) };
	ShapeId const id { addShape( pPipelineNew ) };
	ConnectPipeline( pPipelineNew, id, idStart, idEnd );
	CHECK_CONSISTENCY;
	return id;
}

void NNetModel::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	m_bUnsavedChanges = true;
	switch ( GetShapeType( id ) )
	{
	case tShapeType::pipeline:
	{
		GetTypedShape<BaseKnot>( GetStartKnot( id ) )->MoveShape( delta );
		GetTypedShape<BaseKnot>( GetEndKnot  ( id ) )->MoveShape( delta );
		break;
	}

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
		GetTypedShape<BaseKnot>( id )->MoveShape( delta );
		break;

	default:
//		assert( false );
		break;
	}
	CHECK_CONSISTENCY;
}

ShapeId const NNetModel::InsertNeuron( ShapeId const idPipeline, MicroMeterPoint const & splitPoint )
{
	m_bUnsavedChanges = true;
	ShapeId const idNewNeuron { NewShape<Neuron>( splitPoint ) };
	insertBaseKnot( idPipeline, idNewNeuron );
	CHECK_CONSISTENCY;
	return idNewNeuron;
}

ShapeId const NNetModel::InsertKnot( ShapeId const idPipeline, MicroMeterPoint const & splitPoint )
{
	m_bUnsavedChanges = true;
	ShapeId const idNewKnot { NewShape<Knot>( splitPoint ) };
	insertBaseKnot( idPipeline, idNewKnot );
	CHECK_CONSISTENCY;
	return idNewKnot;
}

void NNetModel::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	NewPipeline( InsertKnot( id, pos ), NewShape<Knot>( pos + orthoVector( id ) ) );
}

void NNetModel::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	NewPipeline( NewShape<Knot>( pos - orthoVector( id ) ), InsertKnot( id, pos ) );
}

void NNetModel::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	NewPipeline( id, NewShape<Knot>( pos + STD_OFFSET) );
}

void NNetModel::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	NewPipeline( NewShape<Knot>( pos - STD_OFFSET ), id );
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

void NNetModel::ResetModel( )
{
	ResetAll( );
	createInitialShapes( );
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

	if ( ! pRes )  // first test all neurons and input neurons
		pRes = FindShapeAt( pnt, [&]( Shape const & s ) { return ::IsAnyNeuronType( s.GetShapeType() ); } );

	if ( ! pRes )   // if nothing found, test knot shapes
		pRes = FindShapeAt( pnt, [&]( Shape const & s ) { return ::IsKnotType( s.GetShapeType() ); } ); 	

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

/////////////////// local functions ///////////////////////////////////////////////

void NNetModel::checkConsistency( Shape const * pShape ) const
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
				assert( IsBaseKnotType( pStart->GetShapeType() ) );
			if ( pEnd )
				assert( IsBaseKnotType( pEnd->GetShapeType() ) );
			break;
		}

	case tShapeType::undefined:
		assert( false );
		break;

	default:
		assert( false );
	}
}

void NNetModel::createInitialShapes( )
{
	ShapeId m_idInputNeuron = NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
	ShapeId m_idNeuron      = NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) );
	ShapeId m_idPipeline    = NewPipeline( m_idInputNeuron, m_idNeuron );
}

void NNetModel::disconnectBaseKnot( BaseKnot * const pBaseKnot )
{
	MicroMeterPoint umPos { pBaseKnot->GetPosition() };
	pBaseKnot->Apply2AllIncomingPipelines
	( 
		[&]( ShapeId const idPipeline ) // every incoming pipeline needs a new end knot
		{ 
			Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
			Knot     * pKnotNew  { new Knot( this, umPos ) };
			pPipeline->SetEndKnot( addShape( pKnotNew ) );
			pKnotNew->AddIncomming( idPipeline );
			pBaseKnot->RemoveIncoming( idPipeline );
		} 
	);
	pBaseKnot->Apply2AllOutgoingPipelines
	( 
		[&]( ShapeId const idPipeline ) // every outgoing pipeline needs a new start knot
		{ 
			Pipeline * pPipeline { GetTypedShape<Pipeline>( idPipeline ) }; 
			Knot     * pKnotNew  { new Knot( this, umPos ) };
			pPipeline->SetStartKnot( addShape( pKnotNew ) );
			pKnotNew->AddOutgoing( idPipeline );
			pBaseKnot->RemoveOutgoing( idPipeline );
		} 
	);
}

void NNetModel::deletePipeline( ShapeId const id )
{
	if ( Pipeline * pPipelineToBeDeleted { GetTypedShape<Pipeline>( id ) } )
	{
		assert( ::IsPipelineType( pPipelineToBeDeleted->GetShapeType( ) ) );

		ShapeId    idStartKnot { pPipelineToBeDeleted->GetStartKnot() };
		BaseKnot * pStartKnot  { GetTypedShape<BaseKnot>( idStartKnot ) };
		if ( pStartKnot )
		{
			pStartKnot->RemoveOutgoing( id );
			if ( pStartKnot->IsOrphanedKnot( ) )
				Disconnect( idStartKnot );
		}

		ShapeId    idEndKnot { pPipelineToBeDeleted->GetEndKnot() };
		BaseKnot * pEndKnot  { GetTypedShape<BaseKnot>( idEndKnot ) };
		if ( pEndKnot )
		{
			pEndKnot->RemoveIncoming( id );
			if ( pEndKnot->IsOrphanedKnot( ) )
				Disconnect( idEndKnot );
		}

		deleteShape( id );
	}
}

ShapeId const NNetModel::addShape( Shape * pShape )
{
	ShapeId id( CastToLong( m_Shapes.size() ) );  
	m_Shapes.push_back( pShape );                 
	pShape->SetId( id );
	return id;
}

void NNetModel::insertBaseKnot( ShapeId const idPipeline, ShapeId const idBaseKnot )
{
	assert( GetConstShape( idBaseKnot )->GetId( ) == idBaseKnot );  // base knot is in list
	Pipeline * const pPipeline   { GetTypedShape<Pipeline>( idPipeline ) };
	ShapeId    const idStartKnot { pPipeline->GetStartKnot() };
	NewPipeline( idStartKnot, idBaseKnot );
	GetTypedShape<BaseKnot>( idStartKnot )->RemoveOutgoing( idPipeline );
	GetTypedShape<BaseKnot>( idBaseKnot  )->AddOutgoing   ( idPipeline );
	pPipeline->SetStartKnot( idBaseKnot );
}

MicroMeterPoint NNetModel::orthoVector( ShapeId const idPipeline ) const
{
	Pipeline const * pPipeline { GetConstTypedShape<Pipeline>( idPipeline ) };
	return OrthoVector( pPipeline->GetVector(), NEURON_RADIUS * 2.f );
}
