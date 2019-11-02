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

float        const NNetModel:: m_dampingFactor      { 0.9995f };    // signal loss per um  
mV           const NNetModel:: m_thresholdPotential {  20._mV };
mV           const NNetModel:: m_peakVoltage        {  10._mV };
microseconds const NNetModel:: m_pulseWidth         {     2ms };
microseconds const NNetModel:: m_refractoryPeriod   {   500us };

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE ),
	m_shapeSuperHighlighted( NO_SHAPE )
{					
	Shape::SetModel( this );

	m_idInputNeuron  = AddInputNeuron ( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) );
    m_idOutputNeuron = AddOutputNeuron( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) );
	m_idPipeline     = AddPipeline    ( 0.1_meterPerSec );

	AddOutgoing ( m_idInputNeuron,  m_idPipeline );
	AddIncomming( m_idOutputNeuron, m_idPipeline );
}

NNetModel::~NNetModel( )
{}

Shape * NNetModel::GetShape( ShapeId const id )
{
	return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() ];
}

Shape const * NNetModel::GetConstShape( ShapeId const id ) const
{
	return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() ];
}

void NNetModel::deleteShape( ShapeId const id )
{
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
	assert( IsBaseKnotType( pShape->GetShapeType() ) );
	return static_cast<BaseKnot *>( pShape );
}

BaseKnot const * NNetModel::GetConstBaseKnot( ShapeId const id ) const
{
	Shape const * pShape = GetConstShape( id );
	assert( pShape );
	assert( IsBaseKnotType( pShape->GetShapeType() ) );
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

void NNetModel::Connect( )  // highlighted knot to super highlighted neuron
{
	if ( (m_shapeHighlighted != NO_SHAPE) && (m_shapeSuperHighlighted != NO_SHAPE) )
	{
		Knot * pKnot = GetKnot( m_shapeHighlighted );
		pKnot->Apply2AllIncomingPipelines
		( 
			[&]( ShapeId const & idPipeline ) 
			{ 
				AddIncomming( m_shapeSuperHighlighted, idPipeline );
			}
		);

		deleteShape( m_shapeHighlighted );
		m_shapeHighlighted = NO_SHAPE;

		SuperHighlightShape( NO_SHAPE );
	}
}

void NNetModel::AddIncomming( ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddIncomming( idPipeline );
	GetPipeline( idPipeline )->SetEndKnot( idBaseKnot );
}

void NNetModel::AddOutgoing( ShapeId const idBaseKnot, ShapeId const idPipeline )
{
	GetBaseKnot( idBaseKnot )->AddOutgoing( idPipeline );
	GetPipeline( idPipeline )->SetStartKnot( idBaseKnot );
}

void NNetModel::HighlightShape( ShapeId const idHighlight )
{
	if ( m_shapeHighlighted != NO_SHAPE )
	{
		Shape * pShape = GetShape( m_shapeHighlighted );
		assert( pShape );
		pShape->SetHighlightState( false );
	}

	if ( idHighlight != NO_SHAPE )
	{
		Shape * pShape = GetShape( idHighlight );
//		assert( pShape );                          //TODO: clarify how this can happen
//		assert( pShape->GetId() == idHighlight );
		if ( pShape && (pShape->GetId() == idHighlight) )
			pShape->SetHighlightState( true );
	}

	m_shapeHighlighted = idHighlight;
}

void NNetModel::SuperHighlightShape( ShapeId const idSuperHighlight )
{
	if ( m_shapeSuperHighlighted != NO_SHAPE )
	{
		GetShape( m_shapeSuperHighlighted )->SetSuperHighlightState( false );
	}

	if ( idSuperHighlight != NO_SHAPE )
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

void NNetModel::CreateNewBranch( ShapeId const idKnot )
{
	Knot          * pKnot         { GetKnot( idKnot ) };
	MicroMeterPoint position      { pKnot->GetPosition() };
	Knot          * pKnotNew      { new Knot( position ) };
	ShapeId const   idNewKnot     { addShape( pKnotNew ) };
	Pipeline      * pPipelineNew  { new Pipeline( ) };
	ShapeId const   idNewPipeline { addShape( pPipelineNew ) };
	AddIncomming( idNewKnot, idNewPipeline );
	AddOutgoing ( idKnot,    idNewPipeline );
	HighlightShape( idNewKnot );
}

void NNetModel::SplitPipeline( ShapeId const idPipeline, MicroMeterPoint const & splitPoint )
{
	Pipeline    * pPipeExisting { GetPipeline( idPipeline ) };
	Pipeline    * pPipeNew      { new Pipeline( ) };
	Knot        * pKnotNew      { new Knot( splitPoint ) };
	ShapeId const idNewKnot     { addShape( pKnotNew ) };
	ShapeId const idNewPipeline { addShape( pPipeNew ) };
	pPipeNew->SetEndKnot( pPipeExisting->GetEndKnot() );
	AddIncomming( idNewKnot, idPipeline );
	AddOutgoing ( idNewKnot, idNewPipeline );
    CreateNewBranch( idNewKnot );
}

void NNetModel::CreateNewNeuron( MicroMeterPoint const & pnt )
{
	ShapeId         const idNewInputNeuron { addShape( new Neuron( pnt ) ) };
	MicroMeterPoint const knotPos          { pnt + MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2 ) };
	ShapeId         const idNewKnot        { addShape( new Knot( knotPos ) ) };
	ShapeId         const idNewPipeline    { addShape( new Pipeline( ) ) };
	AddOutgoing ( idNewInputNeuron, idNewPipeline );
	AddIncomming( idNewKnot,        idNewPipeline );
}

void NNetModel::CreateNewInputNeuron( MicroMeterPoint const & pnt )
{
	ShapeId         const idNewInputNeuron { addShape( new InputNeuron( pnt ) ) };
	MicroMeterPoint const knotPos          { pnt + MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2 ) };
	ShapeId         const idNewKnot        { addShape( new Knot( knotPos ) ) };
	ShapeId         const idNewPipeline    { addShape( new Pipeline( ) ) };
	AddOutgoing ( idNewInputNeuron, idNewPipeline );
	AddIncomming( idNewKnot,        idNewPipeline );
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
		pPipeline->CheckConsistency( );
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
		{
		    func( * pShape );
//			checkConsistency( pShape );
		}
	}
}

void NNetModel::Apply2AllNeurons( std::function<void(Shape &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && (pShape->GetShapeType() != tShapeType::pipeline) )
			func( * pShape );
	}
}

void NNetModel::Apply2AllPipelines( std::function<void(Shape &)> const & func ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape && ( pShape->GetShapeType() == tShapeType::pipeline ))
			func( * pShape );
	}
}

void NNetModel::CopyModelData( ModelInterface const * const src )
{
	* this = * static_cast<NNetModel const *>( src );
}

void NNetModel::Compute( )
{
	Apply2AllShapes( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2AllShapes( [&]( Shape & shape ) { shape.Step( ); } );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::FindShapeUnderPoint( MicroMeterPoint const pnt, std::function<bool(Shape const &)> const & crit ) const
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
				return IsBaseKnotType( shape.GetShapeType() ); 
			} 
	    ); 	

	if ( ! pShapeResult ) 
		pShapeResult = FindShapeUnderPoint   // if nothing found, try pipelines
		( 
			pnt, 
			[&]( Shape const & shape ) 
			{ 
				return ! IsBaseKnotType( shape.GetShapeType() ); 
			} 
	    );

	return pShapeResult;
}
