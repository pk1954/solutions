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
		{ tParameter::pulseWidth,       L"�s"    },
		{ tParameter::signalLoss,       L"1/�m"  },
		{ tParameter::threshold,        L"mV"    },
		{ tParameter::peakVoltage,      L"mV"    },
		{ tParameter::refractoryPeriod, L"�s"    }
	};				  

	return mapParam.at( p );
}

NNetModel::NNetModel( )
  : m_Shapes( ),
	m_timeStamp               ( 0._MicroSecs ),
	m_shapeHighlighted        ( NO_SHAPE ),
	m_shapeSuperHighlighted   ( NO_SHAPE ),
	m_signalLoss              ( 0.0005f ), 
	m_threshold               ( 20._mV ),
	m_peakVoltage             ( 10._mV ),
	m_pulseWidth              ( 2000._MicroSecs ),
	m_refractoryPeriod        ( 500._MicroSecs ),
	m_pulseSpeed              ( 0.1_meterPerSec ),
	m_nrOfInputNeuronsInModel ( 0 ),
	m_nrOfOutputNeuronsInModel( 0 )
{					
	// initial shapes 
	ShapeId m_idInputNeuron  = NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
	ShapeId m_idOutputNeuron = NewShape<OutputNeuron>( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) );
	ShapeId m_idPipeline     = NewPipeline( m_idInputNeuron, m_idOutputNeuron );
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

void NNetModel::RemoveShape( )
{
	if ( HighlightedShapeCanBeDeleted() )
	{
		ShapeId idShapeToBeDeleted { m_shapeHighlighted };
		m_shapeHighlighted = NO_SHAPE;
		BaseKnot * pBaseKnot { GetTypedShape<BaseKnot>( idShapeToBeDeleted ) };
		if ( pBaseKnot->GetNrOfConnections() == 2 )
		{
			long lNrOfShapes = GetNrOfShapes();
			ShapeId idStartKnot{ pBaseKnot->GetPrecursor() };
			ShapeId idEndKnot  { pBaseKnot->GetSuccessor() };
			deleteBaseKnot( idShapeToBeDeleted, false );
			NewPipeline( idStartKnot, idEndKnot );
			assert( GetNrOfShapes() == lNrOfShapes - 2 );
		}
		else 
		{
			deleteBaseKnot( idShapeToBeDeleted, true );
		}
		CHECK_CONSISTENCY;
	}
}

void NNetModel::deleteBaseKnot( ShapeId const id, bool const bRecursive )
{
	BaseKnot * pToBeDeleted { GetTypedShape<BaseKnot>( id ) };
	if ( pToBeDeleted )
	{
		assert( ::IsBaseKnotType( pToBeDeleted->GetShapeType( ) ) );
		m_Shapes[ id.GetValue() ] = nullptr;
		pToBeDeleted->Apply2AllConnectedPipelines( [&]( ShapeId const id ) { deletePipeline( id, bRecursive ); } );
		delete pToBeDeleted;
	}
}

void NNetModel::deletePipeline( ShapeId const id, bool const bRecursive )
{
	Pipeline * pPipelineToBeDeleted { GetTypedShape<Pipeline>( id ) };
	assert( pPipelineToBeDeleted );
	assert( ::IsPipelineType( pPipelineToBeDeleted->GetShapeType( ) ) );
	m_Shapes[ id.GetValue() ] = nullptr;

	ShapeId    idStartKnot { pPipelineToBeDeleted->GetStartKnot() };
	BaseKnot * pStartKnot  { GetTypedShape<BaseKnot>( idStartKnot ) };
	if ( pStartKnot )
	{
		pStartKnot->RemoveOutgoing( id );
		if ( bRecursive && ! pStartKnot->HasOutgoing( ) )
			deleteBaseKnot( idStartKnot, true );
	}

	ShapeId    idEndKnot { pPipelineToBeDeleted->GetEndKnot() };
	BaseKnot * pEndKnot  { GetTypedShape<BaseKnot>( idEndKnot ) };
	if ( pEndKnot )
	{
		pEndKnot->RemoveIncoming( id );
		if ( bRecursive && ! pEndKnot->HasIncoming( ) )
			deleteBaseKnot( idEndKnot, true );
	}

	delete pPipelineToBeDeleted;
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

bool const NNetModel::HighlightedShapeCanBeDeleted( ) const 
{
	switch ( GetHighlightedShapeType( ) )
	{
	case tShapeType::neuron:
	case tShapeType::knot:  
		return GetConstTypedShape<BaseKnot>( m_shapeHighlighted )->GetNrOfConnections( ) == 2;

	case tShapeType::outputNeuron:
		return m_nrOfOutputNeuronsInModel > 1;

	case tShapeType::inputNeuron:
		return m_nrOfInputNeuronsInModel > 1;

	default:
		return false;
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
		assert( pHighlighted->GetShapeType() == tShapeType::outputNeuron );
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

void NNetModel::SplitPipeline( MicroMeterPoint const & splitPoint )
{
	insertNewBaseKnot( new Knot( this, splitPoint ) );
}

void NNetModel::InsertNeuron( MicroMeterPoint const & splitPoint )
{
	insertNewBaseKnot( new Neuron( this, splitPoint ) );
}

void NNetModel::prepareSplit( MicroMeterPoint const & splitPoint )
{
	if ( GetHighlightedShapeType( ) == tShapeType::pipeline )
	{
		insertNewBaseKnot( new Knot( this, splitPoint ) );
	}
}

void NNetModel::AddOutputNeuron( MicroMeterPoint const & splitPoint )
{
	tShapeType const typeHighlighted = GetHighlightedShapeType();
	if ( ( typeHighlighted == tShapeType::pipeline ) || ( typeHighlighted == tShapeType::knot ) )
	{
		prepareSplit( splitPoint );
		ShapeId const idOutputNeuron { NewShape<OutputNeuron>( GetHighlightedShapePos() + STD_OFFSET) };
		NewPipeline( m_shapeHighlighted, idOutputNeuron );
		HighlightShape( idOutputNeuron );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::AddInputNeuron( MicroMeterPoint const & splitPoint )
{
	if ( m_shapeHighlighted != NO_SHAPE )
	{
		prepareSplit( splitPoint );
		ShapeId const idInputNeuron { NewShape<InputNeuron>( GetHighlightedShapePos() - STD_OFFSET ) };
		NewPipeline( idInputNeuron, m_shapeHighlighted );
		HighlightShape( idInputNeuron );
		CHECK_CONSISTENCY;
	}
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
		assert( static_cast<Neuron const *>( pShape )->HasOutgoing() );
		break;

	case tShapeType::outputNeuron:
		assert( ! static_cast<OutputNeuron const *>( pShape )->HasOutgoing() );
		break;

	case tShapeType::pipeline:
		assert( IsType<StartKnotType>( static_cast<Pipeline const *>( pShape )->GetStartKnot() ) );
		assert( IsType<EndKnotType  >( static_cast<Pipeline const *>( pShape )->GetEndKnot  () ) );
		break;

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
