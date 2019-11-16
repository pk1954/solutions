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

using namespace std::chrono;
using std::unordered_map;

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
	m_idInputNeuron  = addShape( new InputNeuron( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) );
	m_idOutputNeuron = addShape( new OutputNeuron( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) );
	m_idPipeline     = addShape( new Pipeline( ) );

	AddOutgoing ( * this, m_idInputNeuron,  m_idPipeline );
	AddIncomming( * this, m_idOutputNeuron, m_idPipeline );
}

NNetModel::~NNetModel( )
{}

void NNetModel::RecalcPipelines( ) 
{ 
	Apply2AllPipelines( [&]( Pipeline & pipe ) { pipe.Recalc( * this ); } );
} 

Shape * NNetModel::GetShape( ShapeId const id )
{
	return ( IsDefined( id ) ) ? m_Shapes[ id.GetValue() ] : nullptr;
}

Shape const * NNetModel::GetConstShape( ShapeId const id ) const
{
	return ( IsDefined( id ) ) ? m_Shapes[ id.GetValue() ] : nullptr;
}

void NNetModel::deleteHighlightedShape( )
{
	ShapeId const id = m_shapeHighlighted;
	m_shapeHighlighted = NO_SHAPE;
	delete m_Shapes[ id.GetValue() ];
	m_Shapes[ id.GetValue() ] = nullptr;
}

Pipeline * NNetModel::GetPipeline( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline *>( pShape );
}

Pipeline const * NNetModel::GetConstPipeline( ShapeId const id ) const
{
	Shape const * pShape { GetConstShape( id ) };
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::pipeline );
	return static_cast<Pipeline const *>( pShape );
}

BaseKnot * NNetModel::GetBaseKnot( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	assert( IsBaseKnotType( id ) );
	return static_cast<BaseKnot *>( pShape );
}

BaseKnot const * NNetModel::GetConstBaseKnot( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	assert( IsBaseKnotType( id ) );
	return static_cast<BaseKnot const *>( pShape );
}

Knot * NNetModel::GetKnot( ShapeId const id ) 
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::knot );
	return static_cast<Knot *>( pShape );
}

Knot const * NNetModel::GetConstKnot( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::knot );
	return static_cast<Knot const *>( pShape );
}

InputNeuron * NNetModel::GetInputNeuron( ShapeId const id )
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron *>( pShape );
}

InputNeuron const * NNetModel::GetConstInputNeuron( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<InputNeuron const *>( pShape );
}

OutputNeuron * NNetModel::GetOutputNeuron( ShapeId const id )
{
	Shape * pShape = GetShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<OutputNeuron *>( pShape );
}

OutputNeuron const * NNetModel::GetConstOutputNeuron( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	assert( pShape->GetShapeType() == tShapeType::inputNeuron );
	return static_cast<OutputNeuron const *>( pShape );
}

float const NNetModel::GetParameterValue
( 
	tParameter const         param,
	Shape      const * const pShape
) const
{
	switch ( param )
	{
	case tParameter::pulseRate:
		return Cast2InputNeuron( pShape )->GetPulseFrequency().GetValue();

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

void const NNetModel::SetParameter( tParameter const param,	float const fNewValue, Shape * const pShape )
{
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

void NNetModel::Connect( NNetModel const & model )  // highlighted knot to super highlighted neuron
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeSuperHighlighted ) )
	{
		Knot * pKnot = GetKnot( m_shapeHighlighted );
		pKnot->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				AddIncomming( model, m_shapeSuperHighlighted, idPipeline );
			}
		);

		deleteHighlightedShape( );
		SuperHighlightShape( NO_SHAPE );
	}
}

void NNetModel::AddIncomming( NNetModel const & model,ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddIncomming( idPipeline );
	GetPipeline( idPipeline )->SetEndKnot( model, idBaseKnot );
}

void NNetModel::AddOutgoing( NNetModel const & model,ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddOutgoing( idPipeline );
	GetPipeline( idPipeline )->SetStartKnot( model, idBaseKnot );
}

void NNetModel::HighlightShape( ShapeId const idHighlight )
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

void NNetModel::CreateNewBranch( ShapeId const idKnot )
{
	Knot          * pKnot         { GetKnot( idKnot ) };
	MicroMeterPoint position      { pKnot->GetPosition() };
	Knot          * pKnotNew      { new Knot( position ) };
	ShapeId const   idNewKnot     { addShape( pKnotNew ) };
	Pipeline      * pPipelineNew  { new Pipeline( ) };
	ShapeId const   idNewPipeline { addShape( pPipelineNew ) };
	AddIncomming( * this, idNewKnot, idNewPipeline );
	AddOutgoing ( * this, idKnot,    idNewPipeline );
	HighlightShape( idNewKnot );
}

void NNetModel::SplitPipeline( ShapeId const idPipeline, MicroMeterPoint const & splitPoint )
{
	Pipeline    * pPipeExisting { GetPipeline( idPipeline ) };
	Pipeline    * pPipeNew      { new Pipeline( ) };
	Knot        * pKnotNew      { new Knot( splitPoint ) };
	ShapeId const idNewKnot     { addShape( pKnotNew ) };
	ShapeId const idNewPipeline { addShape( pPipeNew ) };
	pPipeNew->SetEndKnot( * this, pPipeExisting->GetEndKnot() );
	AddIncomming( * this, idNewKnot, idPipeline );
	AddOutgoing ( * this, idNewKnot, idNewPipeline );
    CreateNewBranch( idNewKnot );
}

void NNetModel::CreateNewNeuron( MicroMeterPoint const & pnt )
{
	ShapeId         const idNewInputNeuron { addShape( new Neuron( pnt ) ) };
	MicroMeterPoint const knotPos          { pnt + MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2 ) };
	ShapeId         const idNewKnot        { addShape( new Knot( knotPos ) ) };
	ShapeId         const idNewPipeline    { addShape( new Pipeline( ) ) };
	AddOutgoing ( * this, idNewInputNeuron, idNewPipeline );
	AddIncomming( * this, idNewKnot,        idNewPipeline );
}

void NNetModel::CreateNewInputNeuron( MicroMeterPoint const & pnt )
{
	ShapeId         const idNewInputNeuron { addShape( new InputNeuron( pnt ) ) };
	MicroMeterPoint const knotPos          { pnt + MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2 ) };
	ShapeId         const idNewKnot        { addShape( new Knot( knotPos ) ) };
	ShapeId         const idNewPipeline    { addShape( new Pipeline( ) ) };
	AddOutgoing ( * this, idNewInputNeuron, idNewPipeline );
	AddIncomming( * this, idNewKnot,        idNewPipeline );
}

void NNetModel::CreateNewOutputNeuron( MicroMeterPoint const & pnt )
{
	addShape( new OutputNeuron( pnt ) );
}

void NNetModel::checkConsistency( Shape * pShape ) const
{
	switch ( pShape->GetShapeType() )
	{
	case tShapeType::inputNeuron:
		break;

	case tShapeType::knot:
		break;

	case tShapeType::neuron:
		break;

	case tShapeType::outputNeuron:
		break;

	case tShapeType::pipeline:
	{
		Pipeline * pPipeline = static_cast<Pipeline *>( pShape );
		pPipeline->CheckConsistency( * this );
	}
		break;

	case tShapeType::undefined:
		assert( false );
		break;

	default:
		assert( false );
	}
}

void NNetModel::Apply2AllShapes( std::function<void(Shape &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape )
		    func( * pShape );
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
		{
			func( static_cast<Pipeline &>( * pShape ) );
		}
	}
}

void NNetModel::Apply2AllParameters( std::function<void(tParameter const &)> const & func ) const
{
	for ( int i = 0; static_cast<tParameter>(i) <= tParameter::refractoryPeriod; ++i )
	{
		func( static_cast<tParameter>( i ) );
	}
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
