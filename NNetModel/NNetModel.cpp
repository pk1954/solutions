// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModel.h"

using namespace std::chrono;

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE )
{					
	m_idNeuron1   = AddInputNeuron( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
    m_idKnot1     = AddKnot       ( MicroMeterPoint( 400.0_MicroMeter, 400.0_MicroMeter ) );
    m_idKnot2     = AddKnot       ( MicroMeterPoint( 300.0_MicroMeter, 800.0_MicroMeter ) );
    m_idKnot3     = AddKnot       ( MicroMeterPoint( 500.0_MicroMeter, 800.0_MicroMeter ) );
    m_idKnot4     = AddKnot       ( MicroMeterPoint( 600.0_MicroMeter, 700.0_MicroMeter ) );
    m_idPipeline1 = AddPipeline   ( 0.1_meterPerSec );
    m_idPipeline2 = AddPipeline   ( 0.1_meterPerSec );
    m_idPipeline3 = AddPipeline   ( 0.1_meterPerSec );
	m_idPipeline4 = AddPipeline   ( 0.1_meterPerSec );

	AddOutgoing ( m_idNeuron1, m_idPipeline1 );
	AddIncomming( m_idKnot1,   m_idPipeline1 );
	AddOutgoing ( m_idKnot1,   m_idPipeline2 );
	AddOutgoing ( m_idKnot1,   m_idPipeline3 );
	AddOutgoing ( m_idKnot3,   m_idPipeline4 );
	AddIncomming( m_idKnot2,   m_idPipeline2 );
	AddIncomming( m_idKnot3,   m_idPipeline3 );
	AddIncomming( m_idKnot4,   m_idPipeline4 );
}

NNetModel::~NNetModel( )
{
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
	if ( m_shapeHighlighted != NO_SHAPE )
	{
		GetShape( m_shapeHighlighted )->SetHighlightState( false );
	}

	if ( idHighlight != NO_SHAPE )
	{
		Shape * pShape = GetShape( idHighlight );
		assert( pShape );
		assert( pShape->GetId() == idHighlight );
		pShape->SetHighlightState( true );
	}

	m_shapeHighlighted = idHighlight;
}

ShapeId const NNetModel::addShape( Shape * pShape )
{
	m_Shapes.push_back( pShape );                        // ShapeId 0 is reserved
	ShapeId id( CastToUnsignedLong( m_Shapes.size() ) );  // after first push_back, size = 1
	pShape->SetId( id );
	return id;
}

ShapeId const NNetModel::AddInputNeuron( MicroMeterPoint const& upCenter )
{
	return addShape( new InputNeuron( upCenter ) );
}

ShapeId const NNetModel::AddNeuron( MicroMeterPoint const & upCenter )
{
	return addShape( new Neuron( upCenter ) );
}

ShapeId const NNetModel::AddKnot( MicroMeterPoint const & upCenter )
{
	return addShape( new Knot( upCenter ) );
}

ShapeId const NNetModel::AddPipeline( meterPerSec const impulseSpeed )
{
	return addShape( new Pipeline( impulseSpeed ) );
}

void NNetModel::Apply2AllShapes( std::function<void(Shape * const)> const & func ) const
{
	for ( auto shape : m_Shapes )
	{
		func( shape );
	}
}

void NNetModel::CopyModelData( ModelInterface const * const src )
{
	* this = * static_cast<NNetModel const *>( src );
}

void NNetModel::Compute( )
{
	Apply2AllShapes( [&]( Shape * const pShape ) { pShape->Prepare( * this ); } );
	Apply2AllShapes( [&]( Shape * const pShape ) { pShape->Step(); } );

	m_timeStamp += TIME_RESOLUTION;

	////////////////
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::GetShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape->IsPointInShape( * this, pnt ) ) 
			return pShape;
	};

	return nullptr;
}
