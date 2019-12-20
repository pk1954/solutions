// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "NNetParameters.h"
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
	m_shapeHighlighted     ( NO_SHAPE ),
	m_shapeSuperHighlighted( NO_SHAPE ),
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


void NNetModel::RemoveShape( )
{
	ShapeId idShapeToBeDeleted { m_shapeHighlighted };
	m_shapeHighlighted = NO_SHAPE;
	deleteBaseKnot( idShapeToBeDeleted );
	CHECK_CONSISTENCY;
}

void NNetModel::deleteBaseKnot( ShapeId const id )
{
	BaseKnot * pToBeDeleted { GetTypedShape<BaseKnot>( id ) };
	if ( pToBeDeleted )
	{
		assert( ::IsBaseKnotType( pToBeDeleted->GetShapeType( ) ) );
		m_Shapes[ id.GetValue() ] = nullptr;
		pToBeDeleted->Apply2AllConnectedPipelines( [&]( ShapeId const id ) { deletePipeline( id ); } );
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

float const NNetModel::GetParameterValue
( 
	tParameter const   param,
	Shape      const * pShape
) const
{
	switch ( param )
	{
	case tParameter::pulseRate:
	{
		if ( ! pShape )
			pShape = GetConstShape( m_shapeHighlighted );
		assert( pShape );
		return Cast2InputNeuron( pShape )->GetPulseFrequency().GetValue();
	}

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

void const NNetModel::SetParameter
( 
	ShapeId    const id, 
	tParameter const param, 
	float      const fNewValue 
)
{
	switch ( param )
	{
	case tParameter::pulseRate:
		{
			InputNeuron * const pInputNeuron { GetTypedShape<InputNeuron>( id ) };
			if ( pInputNeuron )
				pInputNeuron->SetPulseFrequency( static_cast< fHertz >( fNewValue ) );
		}
		break;

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

ShapeId const NNetModel::HighlightShape( ShapeId const id ) 
{
	ShapeId idOld = m_shapeHighlighted;
	m_shapeHighlighted = id;
	return idOld;
};

ShapeId const NNetModel::SuperHighlightShape( ShapeId const id ) 
{ 
	ShapeId idOld = m_shapeSuperHighlighted;
	m_shapeSuperHighlighted = id;
	return idOld;
}

tShapeType const NNetModel::GetHighlightedShapeType( ) const 
{ 
	Shape const * const pHighlighted{ GetConstShape( m_shapeHighlighted ) };
	return ( pHighlighted ) ? pHighlighted->GetShapeType( ) : tShapeType::undefined;
}

tShapeType const NNetModel::GetSuperHighlightedShapeType( ) const 
{ 
	Shape const * const pSuperHighlighted{ GetConstShape( m_shapeSuperHighlighted ) };
	return ( pSuperHighlighted ) ? pSuperHighlighted->GetShapeType( ) : tShapeType::undefined;
}

MicroMeterPoint const NNetModel::GetHighlightedShapePos( ) const 
{ 
	return IsDefined( m_shapeHighlighted ) 
		? GetConstTypedShape<BaseKnot>( m_shapeHighlighted )->GetPosition()
		: NP_NULL;
}

void NNetModel::Connect( )  // merge highlighted shape into super highlighted shape
{
	long lNrOfShapes = GetNrOfShapes();
	BaseKnot * pHighlighted      = GetTypedShape<BaseKnot>( m_shapeHighlighted );
	BaseKnot * pSuperHighlighted = GetTypedShape<BaseKnot>( m_shapeSuperHighlighted );
	if ( pHighlighted && pSuperHighlighted )
	{
		assert( NeuronType::TypeFits( pSuperHighlighted->GetShapeType() ) );
		pHighlighted->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				pHighlighted->RemoveIncoming( idPipeline );
				pSuperHighlighted->AddIncomming( idPipeline );
				GetTypedShape<Pipeline>(idPipeline)->SetEndKnot( m_shapeSuperHighlighted );
			}
		);

		RemoveShape( );
		SuperHighlightShape( NO_SHAPE );
	}
	assert( GetNrOfShapes() == lNrOfShapes - 1 );
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

void NNetModel::insertNewBaseKnot( BaseKnot * const pNewBaseKnot )
{
	Pipeline * const pPipelineHighlighted { GetTypedShape<Pipeline>( m_shapeHighlighted )};
	if ( pPipelineHighlighted )
	{
		ShapeId const idStartKnot{ pPipelineHighlighted->GetStartKnot() };
		ShapeId const idNewShape { addShape( pNewBaseKnot ) };
		NewPipeline( idStartKnot, idNewShape );
		GetTypedShape<BaseKnot>( idStartKnot )->RemoveOutgoing( m_shapeHighlighted );
		GetTypedShape<BaseKnot>( idNewShape  )->AddOutgoing   ( m_shapeHighlighted );
		pPipelineHighlighted->SetStartKnot( idNewShape );
		HighlightShape( idNewShape );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::MoveShape( MicroMeterPoint const & delta )
{
	switch ( GetHighlightedShapeType( ) )
	{
	case tShapeType::pipeline:
	{
		Pipeline * const pPipelineHighlighted { GetTypedShape<Pipeline>( m_shapeHighlighted )};
		GetTypedShape<BaseKnot>( pPipelineHighlighted->GetStartKnot() )->MoveShape( delta );
		GetTypedShape<BaseKnot>( pPipelineHighlighted->GetEndKnot  () )->MoveShape( delta );
		break;
	}

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
	{
		GetTypedShape<BaseKnot>( GetHighlightedShapeId( ) )->MoveShape( delta );
		break;
	}

	default:
		assert( false );
	}
}

void NNetModel::SplitPipeline( MicroMeterPoint const & splitPoint )
{
	insertNewBaseKnot( new Knot( this, splitPoint ) );
}

void NNetModel::InsertNeuron( MicroMeterPoint const & splitPoint )
{
	insertNewBaseKnot( new Neuron( this, splitPoint ) );
}

void NNetModel::AddOutgoing( MicroMeterPoint const & pos )
{
	ShapeId idNewKnot { NO_SHAPE };
	switch ( GetHighlightedShapeType( ) )
	{
	case tShapeType::pipeline:
		insertNewBaseKnot( new Knot( this, pos ) );
		[[fallthrough]]; 

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
	{
		idNewKnot = NewShape<Knot>( pos );
		NewPipeline( m_shapeHighlighted, idNewKnot );
		break;
	}

	default:
		assert( false );
	}

	HighlightShape( idNewKnot );
	CHECK_CONSISTENCY;
}

void NNetModel::AddIncoming( MicroMeterPoint const & pos )
{
	ShapeId idNewKnot { NO_SHAPE };
	switch ( GetHighlightedShapeType( ) )
	{
	case tShapeType::pipeline:
		insertNewBaseKnot( new Knot( this, pos ) );
		[[fallthrough]]; 

	case tShapeType::inputNeuron:
	case tShapeType::neuron:
	case tShapeType::knot:
	{
		idNewKnot = NewShape<Knot>( pos );
		NewPipeline( idNewKnot, m_shapeHighlighted );
		break;
	}

	default:
		assert( false );
	}

	HighlightShape( idNewKnot );
	CHECK_CONSISTENCY;
}

void NNetModel::AddNeuron( MicroMeterPoint const & pos )
{
	ShapeId idNeuron;

	switch ( GetHighlightedShapeType( ) )
	{
		case tShapeType::pipeline:
			insertNewBaseKnot( new Knot( this, pos ) );
			[[fallthrough]]; 

		case tShapeType::knot:
			idNeuron = NewShape<Neuron>( pos + STD_OFFSET);
			NewPipeline( m_shapeHighlighted, idNeuron );
			break;

		case tShapeType::undefined:
			idNeuron = NewShape<Neuron>( pos );
			break;

		default:
			assert( false );
	}

	HighlightShape( idNeuron );
	CHECK_CONSISTENCY;
}

void NNetModel::AddInputNeuron( MicroMeterPoint const & pos )
{
	ShapeId idInputNeuron { NO_SHAPE };
	switch ( GetHighlightedShapeType( ) )
	{
	case tShapeType::pipeline:
		insertNewBaseKnot( new Knot( this, pos ) );
		[[fallthrough]]; 

	case tShapeType::knot:
	case tShapeType::neuron:
		idInputNeuron = NewShape<InputNeuron>( GetHighlightedShapePos() - STD_OFFSET );
		NewPipeline( idInputNeuron, m_shapeHighlighted );
		HighlightShape( idInputNeuron );
		break;

	case tShapeType::undefined:
	{
		ShapeId const idEndKnot { NewShape<Knot>( pos ) };
		idInputNeuron = NewShape<InputNeuron>( pos - STD_OFFSET );
		NewPipeline( idInputNeuron, idEndKnot );
		HighlightShape( idEndKnot );
	}
	break;

	default:
		assert( false );
	}

	CHECK_CONSISTENCY;
}

void NNetModel::CheckConsistency( Shape const * pShape ) const
{
	tShapeType type = pShape->GetShapeType();

	if ( ::IsBaseKnotType( type ) )
		static_cast<BaseKnot const &>( * pShape ).Apply2AllConnectedPipelinesConst
		( 
			[&]( ShapeId const id ) 
			{ 
				assert( IsType<Pipeline>( id ) ); 
			} 
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
	Apply2All<Shape>
	( 
		[&]( Shape & shape ) 
		{ 
			delete & shape; 
		} 
	);
	m_Shapes.clear();
}

Shape const * NNetModel::FindShapeUnderPoint( MicroMeterPoint const pnt, function<bool(Shape const &)> const & crit ) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape;
	};

	return nullptr;
}

Shape const * NNetModel::FindShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	Shape const * pShapeResult = FindShapeUnderPoint  // first test all knot shapes
	    ( 
			pnt, 
			[&]( Shape const & shape ) 
			{ 
				return ::IsBaseKnotType( shape.GetShapeType() ); 
			} 
	    ); 	

	if ( ! pShapeResult ) 
		pShapeResult = FindShapeUnderPoint   // if nothing found, try pipelines
		( 
			pnt, 
			[&]( Shape const & shape ) 
			{ 
				return ! ::IsBaseKnotType( shape.GetShapeType() ); 
			} 
	    );

	return pShapeResult;
}

COLORREF const NNetModel::GetFrameColor( Shape const & shape ) const 
{ 
	return IsSuperHighlighted( shape ) 
		? EXT_COLOR_SUPER_HIGHLIGHT 
		: IsHighlighted( shape ) 
		? EXT_COLOR_HIGHLIGHT 
		: EXT_COLOR_NORMAL;
};
