// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include <vector>
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
	m_idInputNeuron1  = AddInputNeuron ( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
    m_idKnot1         = AddKnot        ( MicroMeterPoint( 400.0_MicroMeter, 400.0_MicroMeter ) );
	m_idNeuron1       = AddNeuron      ( MicroMeterPoint( 500.0_MicroMeter, 800.0_MicroMeter ) );
    m_idPipeline1     = AddPipeline    ( 0.1_meterPerSec );
    m_idPipeline2     = AddPipeline    ( 0.1_meterPerSec );
    m_idPipeline3     = AddPipeline    ( 0.1_meterPerSec );
	m_idPipeline4     = AddPipeline    ( 0.1_meterPerSec );
    m_idOutputNeuron1 = AddOutputNeuron( MicroMeterPoint( 300.0_MicroMeter, 800.0_MicroMeter ) );

	AddOutgoing ( m_idInputNeuron1,  m_idPipeline1 );
	AddIncomming( m_idKnot1,         m_idPipeline1 );
	AddOutgoing ( m_idKnot1,         m_idPipeline2 );
	AddOutgoing ( m_idKnot1,         m_idPipeline3 );
	AddOutgoing ( m_idNeuron1,       m_idPipeline4 );
	AddIncomming( m_idOutputNeuron1, m_idPipeline2 );
	AddIncomming( m_idNeuron1,       m_idPipeline3 );
	AddIncomming( m_idOutputNeuron1, m_idPipeline4 );
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
	m_Shapes.push_back( pShape );                         // ShapeId 0 is reserved
	ShapeId id( CastToUnsignedLong( m_Shapes.size() ) );  // after first push_back, size = 1
	pShape->SetId( id );
	return id;
}

ShapeId const NNetModel::AddInputNeuron( MicroMeterPoint const& upCenter )
{
	return addShape( new InputNeuron( upCenter ) );
}

ShapeId const NNetModel::AddOutputNeuron( MicroMeterPoint const& upCenter )
{
	return addShape( new OutputNeuron( upCenter ) );
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

void NNetModel::CreateNewBranch( ShapeId const id )
{
	Knot    const * pKnot         { GetConstKnot( id ) };
	ShapeId const   idNewKnot     { addShape( new Knot( pKnot->GetPosition() ) ) };
	ShapeId const   idNewPipeline { addShape( new Pipeline( ) ) };
	Pipeline      * newPipeline   { GetPipeline( idNewPipeline ) };
	newPipeline->SetStartKnot( * this, id );
	newPipeline->SetEndKnot  ( * this, idNewKnot );
}

void NNetModel::CreateNewNeuron( MicroMeterPoint const & pnt )
{
	ShapeId const idNewNeuron   { addShape( new Neuron( pnt ) ) };
	ShapeId const idNewKnot     { addShape( new Knot  ( pnt ) ) };
	ShapeId const idNewPipeline { addShape( new Pipeline( ) ) };
	Pipeline    * newPipeline   { GetPipeline( idNewPipeline ) };
	newPipeline->SetStartKnot( * this, idNewNeuron );
	newPipeline->SetEndKnot  ( * this, idNewKnot );
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
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::GetShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	// iterate in reverse order, so that newer shapes are checked first

	for ( size_t i = m_Shapes.size(); i --> 0; )	// first test all knot shapes
	{
		if ( IsBaseKnotType( m_Shapes[i]->GetShapeType() ) &&  m_Shapes[i]->IsPointInShape( * this, pnt ) ) 
			return m_Shapes[i];
	};

	for ( size_t i = m_Shapes.size(); i --> 0; )	 // now try pipelines
	{
		if ( ! IsBaseKnotType( m_Shapes[i]->GetShapeType() ) &&  m_Shapes[i]->IsPointInShape( * this, pnt ) ) 
			return m_Shapes[i];
	};

	return nullptr;
}
