// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Knot.h"
#include "NNetModel.h"

using namespace std::chrono;

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE )
{
	InputNeuron * pNeuron1   = new InputNeuron( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
	Knot        * pKnot1     = new Knot( MicroMeterPoint( 400.0_MicroMeter, 400.0_MicroMeter ) );
	Knot        * pKnot2     = new Knot( MicroMeterPoint( 300.0_MicroMeter, 800.0_MicroMeter ) );
	Knot        * pKnot3     = new Knot( MicroMeterPoint( 500.0_MicroMeter, 800.0_MicroMeter ) );
	Knot        * pKnot4     = new Knot( MicroMeterPoint( 600.0_MicroMeter, 700.0_MicroMeter ) );
	Pipeline    * pPipeline1 = new Pipeline( 0.1_meterPerSec );
	Pipeline    * pPipeline2 = new Pipeline( 0.1_meterPerSec );
	Pipeline    * pPipeline3 = new Pipeline( 0.1_meterPerSec );
	Pipeline    * pPipeline4 = new Pipeline( 0.1_meterPerSec );

	m_idKnot1     = AddShape( * pKnot1 );
	m_idKnot2     = AddShape( * pKnot2 );
	m_idKnot3     = AddShape( * pKnot3 );
	m_idKnot4     = AddShape( * pKnot4 );
	m_idPipeline1 = AddShape( * pPipeline1 );
	m_idPipeline2 = AddShape( * pPipeline2 );
	m_idPipeline3 = AddShape( * pPipeline3 );
	m_idPipeline4 = AddShape( * pPipeline4 );
	m_idNeuron1   = AddShape( * pNeuron1 );

	pNeuron1->AddOutgoing( * this, m_idPipeline1 );
	pKnot1->AddIncomming ( * this, m_idPipeline1 );
	pKnot1->AddOutgoing  ( * this, m_idPipeline2 );
	pKnot1->AddOutgoing  ( * this, m_idPipeline3 );
	pKnot3->AddOutgoing  ( * this, m_idPipeline4 );
	pKnot2->AddIncomming ( * this, m_idPipeline2 );
	pKnot3->AddIncomming ( * this, m_idPipeline3 );
	pKnot4->AddIncomming ( * this, m_idPipeline4 );
}

NNetModel::~NNetModel( )
{
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

ShapeId const NNetModel::AddShape( Shape & shape )
{
	m_Shapes.push_back( & shape );                        // ShapeId 0 is reserved
	ShapeId id( CastToUnsignedLong( m_Shapes.size() ) );  // after first push_back, size = 1
	shape.SetId( id );
	return id;
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
