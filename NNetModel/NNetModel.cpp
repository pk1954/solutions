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

NNetModel::ModelInconsistencyException const NNetModel::ModelInconsistency;

wchar_t const * const NNetModel::GetParameterName( tParameter const p ) const
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,        L"Pulse rate"        },
		{ tParameter::pulseSpeed,       L"Pulse speed"       },
		{ tParameter::pulseWidth,       L"Pulse width"       },
		{ tParameter::dampingFactor,    L"Damping factor"    },
		{ tParameter::threshold,        L"Threshold"         },
		{ tParameter::peakVoltage,      L"Peak voltage"      },
		{ tParameter::refractoryPeriod, L"Refractory period" }
	};				  

	return mapParam.at( p );
}

wchar_t const * const NNetModel::GetParameterLiteral( tParameter const p ) const
{
	static unordered_map < tParameter, wchar_t const * const > mapParam =
	{
		{ tParameter::pulseRate,        L"tParameter::pulseRate"        },
		{ tParameter::pulseSpeed,       L"tParameter::pulseSpeed"       },
		{ tParameter::pulseWidth,       L"tParameter::pulseWidth"       },
		{ tParameter::dampingFactor,    L"tParameter::dampingFactor"    },
		{ tParameter::threshold,        L"tParameter::threshold"        },
		{ tParameter::peakVoltage,      L"tParameter::peakVoltage"      },
		{ tParameter::refractoryPeriod, L"tParameter::refractoryPeriod" }
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
		{ tParameter::dampingFactor,    L"1/µm"  },
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
	m_dampingFactor        ( 0.9995f ), 
	m_threshold            ( 20._mV ),
	m_peakVoltage          ( 10._mV ),
	m_pulseWidth           ( 2000._MicroSecs ),
	m_refractoryPeriod     ( 500._MicroSecs ),
	m_pulseSpeed           ( 0.1_meterPerSec )
{					
	// initial shapes 
	ShapeId m_idInputNeuron  = NewInputNeuron( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
	ShapeId m_idOutputNeuron = NewOutputNeuron( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) );
	ShapeId m_idPipeline     = NewPipeline( m_idInputNeuron, m_idOutputNeuron );
}

NNetModel::~NNetModel( )
{}

void NNetModel::RecalcPipelines( ) 
{ 
	Apply2AllPipelines( [&]( Pipeline & pipe ) { pipe.Recalc( * this ); } );
} 

void NNetModel::DeleteShape( )
{
	if ( IsDefined( m_shapeHighlighted ) )
	{
		tShapeType const type { GetShape( m_shapeHighlighted )->GetShapeType( ) };
		if ( ::HasAxon( type ) )
		{
			deleteShape( GetConstBaseKnot( m_shapeHighlighted )->GetAxon( ) );
		}
		else if ( ::IsPipelineType( type ) )
		{
			ShapeId idStartKnot { GetConstPipeline( m_shapeHighlighted )->GetStartKnot() };
			if ( HasAxon( idStartKnot ) )
				deleteShape( idStartKnot );
		}
		deleteShape( m_shapeHighlighted );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::deleteShape( ShapeId const id )
{
	Shape * pShapeToBeDeleted = m_Shapes[ id.GetValue() ];
//	m_Shapes[ id.GetValue() ] = nullptr;
	m_Shapes.erase( m_Shapes.begin() + id.GetValue() );
	Apply2AllShapes( [&]( Shape & shape ) { shape.FixShapeIds( id ); } );
	FixShapeId( m_shapeHighlighted,      id );
	FixShapeId( m_shapeSuperHighlighted, id );
//	FixShapeId( m_shapeSelected,         id );
	delete pShapeToBeDeleted;
}

Shape * NNetModel::GetShape( ShapeId const id )
{
	if ( id.GetValue() >= m_Shapes.size() )
		throw ModelInconsistency;
	return ( IsDefined( id ) ) ? m_Shapes[ id.GetValue() ] : nullptr;
}

Shape const * NNetModel::GetConstShape( ShapeId const id ) const
{
	if ( id.GetValue() >= m_Shapes.size() )
		throw ModelInconsistency;
	return ( IsDefined( id ) ) ? m_Shapes[ id.GetValue() ] : nullptr;
}

Pipeline * NNetModel::GetPipeline( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::pipeline ) 
		throw ModelInconsistency;
	return static_cast<Pipeline *>( pShape );
}

Pipeline const * NNetModel::GetConstPipeline( ShapeId const id ) const
{
	Shape const * pShape { GetConstShape( id ) };
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::pipeline ) 
		throw ModelInconsistency;
	return static_cast<Pipeline const *>( pShape );
}

BaseKnot * NNetModel::GetBaseKnot( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	if ( ! IsBaseKnotType( id ) ) 
		throw ModelInconsistency;
	return static_cast<BaseKnot *>( pShape );
}

BaseKnot const * NNetModel::GetConstBaseKnot( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	if ( ! IsBaseKnotType( id ) ) 
		throw ModelInconsistency;
	return static_cast<BaseKnot const *>( pShape );
}

Knot * NNetModel::GetKnot( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	if ( ! IsBaseKnotType( id ) ) 
		throw ModelInconsistency;
	return static_cast<Knot *>( pShape );
}

Knot const * NNetModel::GetConstKnot( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	if ( ! IsBaseKnotType( id ) ) 
		throw ModelInconsistency;
	return static_cast<Knot const *>( pShape );
}

InputNeuron * NNetModel::GetInputNeuron( ShapeId const id )
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::inputNeuron ) 
		throw ModelInconsistency;
	return static_cast<InputNeuron *>( pShape );
}

InputNeuron const * NNetModel::GetConstInputNeuron( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::inputNeuron ) 
		throw ModelInconsistency;
	return static_cast<InputNeuron const *>( pShape );
}

OutputNeuron * NNetModel::GetOutputNeuron( ShapeId const id )
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::outputNeuron ) 
		throw ModelInconsistency;
	return static_cast<OutputNeuron *>( pShape );
}

OutputNeuron const * NNetModel::GetConstOutputNeuron( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	if ( pShape->GetShapeType() != tShapeType::outputNeuron ) 
		throw ModelInconsistency;
	return static_cast<OutputNeuron const *>( pShape );
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

	case tParameter::dampingFactor:
		return m_dampingFactor;

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

void const NNetModel::SetParameter( tParameter const param,	float const fNewValue )
{
	if ( IsDefined( m_shapeHighlighted ) )
	{
		Shape * pShape { GetShape( m_shapeHighlighted ) };
		switch ( param )
		{
		case tParameter::pulseRate:
			Cast2InputNeuron( pShape )->SetPulseFrequency( static_cast< fHertz >( fNewValue ) );
			break;

		case tParameter::pulseSpeed:
			m_pulseSpeed = static_cast< meterPerSec >( fNewValue );
			RecalcPipelines( );
			break;

		case tParameter::pulseWidth:
			m_pulseWidth = static_cast< MicroSecs >( fNewValue );
			break;

		case tParameter::dampingFactor:
			m_dampingFactor = fNewValue;
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
}

void NNetModel::Connect( )  // highlighted knot to super highlighted neuron
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeSuperHighlighted ) )
	{
		assert( IsKnotType  ( m_shapeHighlighted ) );
		assert( IsNeuronType( m_shapeSuperHighlighted ) );
		Knot * pKnot = GetKnot( m_shapeHighlighted );
		pKnot->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				AddIncomming( m_shapeSuperHighlighted, idPipeline );
			}
		);

		deleteShape( m_shapeHighlighted );
		SuperHighlightShape( NO_SHAPE );
	}
	CHECK_CONSISTENCY;
}

void NNetModel::AddIncomming( ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddIncomming( idPipeline );
	GetPipeline( idPipeline )->SetEndKnot( * this, idBaseKnot );
}

void NNetModel::AddOutgoing( ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddOutgoing( idPipeline );
	GetPipeline( idPipeline )->SetStartKnot( * this, idBaseKnot );
}

void NNetModel::HighlightShape( ShapeId const idHighlight )
{
	if ( m_shapeHighlighted != idHighlight )
	{
		if ( IsDefined( m_shapeHighlighted ) )
		{
			Shape * pShape = GetShape( m_shapeHighlighted );
			assert( pShape );
			pShape->SetHighlightState( false );
		}

		if ( IsDefined( idHighlight ) )
		{
			Shape * pShape = GetShape( idHighlight );
			assert( pShape );                        
			assert( pShape->GetId() == idHighlight );
			if ( pShape && (pShape->GetId() == idHighlight) )
				pShape->SetHighlightState( true );
		}

		m_shapeHighlighted = idHighlight;
	}
}

void NNetModel::SuperHighlightShape( ShapeId const idSuperHighlight )
{
	if ( IsDefined( m_shapeSuperHighlighted ) )
	{
		GetShape( m_shapeSuperHighlighted )->SetSuperHighlightState( false );
	}

	if ( IsDefined( idSuperHighlight ) )
	{
		Shape * pShape = GetShape( idSuperHighlight );
		assert( pShape );
		assert( pShape->GetId() == idSuperHighlight );
		pShape->SetSuperHighlightState( true );
	}

	m_shapeSuperHighlighted = idSuperHighlight;
}

ShapeId const NNetModel::addShape( Shape * pShape )
{
	ShapeId id( CastToLong( m_Shapes.size() ) );  
	m_Shapes.push_back( pShape );                 
	pShape->SetId( id );
	return id;
}

ShapeId NNetModel::NewInputNeuron( MicroMeterPoint const & pos )
{
	return addShape( new InputNeuron( pos ) );
}

ShapeId NNetModel::NewOutputNeuron( MicroMeterPoint const & pos )
{
	return addShape( new OutputNeuron( pos ) );
}

ShapeId NNetModel::NewNeuron( MicroMeterPoint const & pos )
{
	return addShape( new Neuron( pos ) );
}

ShapeId NNetModel::NewKnot( MicroMeterPoint const & pos )
{
	return addShape( new Knot( pos ) );
}

void NNetModel::RemovePipeline( ShapeId const idPipeline )
{
	Pipeline * pPipeline   { GetPipeline( idPipeline ) };
	ShapeId    idStartKnot { pPipeline->GetStartKnot() };
	ShapeId    idEndKnot   { pPipeline->GetEndKnot  () };
	Knot     * pStartKnot  { GetKnot( idStartKnot ) };
	Knot     * pEndKnot    { GetKnot( idEndKnot ) };
	pStartKnot->RemoveOutgoing( idPipeline );
	pEndKnot  ->RemoveIncoming( idPipeline );
	if ( pStartKnot->IsOrphan() )
		deleteShape( idStartKnot );
	if ( pEndKnot->IsOrphan() )
		deleteShape( idEndKnot );
	deleteShape( idPipeline );
	CHECK_CONSISTENCY;
}

ShapeId NNetModel::NewPipeline( ShapeId const idStart, ShapeId const idEnd )
{
	ShapeId const id { addShape( new Pipeline( ) ) };
	AddOutgoing ( idStart, id );
	AddIncomming( idEnd,   id );
	CHECK_CONSISTENCY;
	return id;
}

void NNetModel::SplitPipeline( MicroMeterPoint const & splitPoint )
{
	ShapeId    const idOldPipeline  { m_shapeHighlighted };
	Pipeline * const pOldPipeline   { GetPipeline( idOldPipeline ) };
	ShapeId    const idNewKnot1     { NewKnot( splitPoint ) };
	ShapeId    const idNewPipeline1 { NewPipeline( pOldPipeline->GetStartKnot(), idNewKnot1                 ) };
	ShapeId    const idNewPipeline2 { NewPipeline( idNewKnot1,                   pOldPipeline->GetEndKnot() ) };
	ShapeId    const idNewKnot2     { NewKnot( GetKnot( idNewKnot1 )->GetPosition() ) };
	ShapeId    const idNewPipeline3 { NewPipeline( idNewKnot1, idNewKnot2 ) };
	RemovePipeline( idOldPipeline );
	HighlightShape( idNewKnot2 );
	CHECK_CONSISTENCY;
}

void NNetModel::createAxon( ShapeId const idNeuron )
{
	MicroMeterPoint const neuronPos     { GetConstBaseKnot( idNeuron)->GetPosition( ) };
	MicroMeterPoint const knotPos       { neuronPos + MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2 ) };
	ShapeId         const idNewKnot     { NewKnot( knotPos ) };
	ShapeId         const idNewPipeline { NewPipeline( idNeuron, idNewKnot ) };
	CHECK_CONSISTENCY;
}

void NNetModel::CreateNewNeuron( MicroMeterPoint const & pnt )
{
	createAxon( NewNeuron( pnt ) );
}

void NNetModel::CreateNewInputNeuron( MicroMeterPoint const & pnt )
{
	createAxon( NewInputNeuron( pnt ) );
}

void NNetModel::CreateNewOutputNeuron( MicroMeterPoint const & pnt )
{
	NewOutputNeuron( pnt ) ;
}

void NNetModel::CheckConsistency( Shape const * pShape ) const
{
	tShapeType type = pShape->GetShapeType();

	if ( ::IsBaseKnotType( type ) )
		static_cast<BaseKnot const &>( * pShape ).Apply2AllConnectedPipelinesConst
		( 
			[&]( ShapeId const id ) 
			{ 
				assert( IsPipelineType( id ) ); 
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
		assert( IsStartKnotType( static_cast<Pipeline const *>( pShape )->GetStartKnot() ) );
		assert( IsEndKnotType  ( static_cast<Pipeline const *>( pShape )->GetEndKnot  () ) );
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
	Apply2AllShapes( [&]( Shape & shape ) { CheckConsistency( & shape ); } );
}

void NNetModel::Apply2AllShapes( std::function<void(Shape &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape )
			func( * pShape );
	}
}

void NNetModel::Apply2AllBaseKnots( std::function<void(BaseKnot &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && ( ::IsBaseKnotType( pShape->GetShapeType() ) ) )
			func( static_cast<BaseKnot &>( * pShape ) );
	}
}

void NNetModel::Apply2AllNeurons( std::function<void(Neuron &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && (pShape->GetShapeType() != tShapeType::pipeline) )
			func( static_cast<Neuron &>( * pShape ) );
	}
}

void NNetModel::Apply2AllInputNeurons( std::function<void(InputNeuron &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && (pShape->GetShapeType() == tShapeType::inputNeuron) )
			func( static_cast<InputNeuron &>( * pShape ) );
	}
}

void NNetModel::Apply2AllPipelines( std::function<void(Pipeline &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && ( pShape->GetShapeType() == tShapeType::pipeline ))
			func( static_cast<Pipeline &>( * pShape ) );
	}
}

void NNetModel::Apply2AllParameters( std::function<void(tParameter const &)> const & func ) const
{
	for ( int i = 0; static_cast<tParameter>(i) <= tParameter::tParameterLast; ++i )
		func( static_cast<tParameter>( i ) );
}

void NNetModel::CopyModelData( ModelInterface const * const src )
{
	* this = * static_cast<NNetModel const *>( src );
}

void NNetModel::Compute( )
{
	Apply2AllShapes( [&]( Shape & shape ) { shape.Prepare( * this ); } );
	Apply2AllShapes( [&]( Shape & shape ) { shape.Step   ( * this ); } );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{
	Apply2AllShapes( [&]( Shape & shape ) { delete & shape; } );
	m_Shapes.clear();
}

Shape const * NNetModel::FindShapeUnderPoint( MicroMeterPoint const pnt, std::function<bool(Shape const &)> const & crit ) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( * this, pnt ) ) 
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
