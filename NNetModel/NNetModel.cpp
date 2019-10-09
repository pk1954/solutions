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
	Pipeline    * pPipeline1 = new Pipeline( 0.1_meterPerSec );
	Pipeline    * pPipeline2 = new Pipeline( 0.1_meterPerSec );
	Pipeline    * pPipeline3 = new Pipeline( 0.1_meterPerSec );

	AddShape( * pNeuron1 );
	AddShape( * pKnot1 );
	AddShape( * pKnot2 );
	AddShape( * pKnot3 );
	AddShape( * pPipeline1 );
	AddShape( * pPipeline2 );
	AddShape( * pPipeline3 );

	pNeuron1->AddOutgoing( pPipeline1 );
	pKnot1->AddIncomming ( pPipeline1 );
	pKnot1->AddOutgoing  ( pPipeline2 );
	pKnot1->AddOutgoing  ( pPipeline3 );
	pKnot2->AddIncomming ( pPipeline2 );
	pKnot3->AddIncomming ( pPipeline3 );
}

NNetModel * NNetModel::CreateModel( )
{
	return new NNetModel( );
}

void NNetModel::DestroyModel( NNetModel * pModel )
{
	delete pModel;
}

void NNetModel::Compute( )
{
	Apply2AllShapes( [&]( Shape * const pShape ) { pShape->Prepare(); } );
	Apply2AllShapes( [&]( Shape * const pShape ) { pShape->Step(); } );

	m_timeStamp += TIME_RESOLUTION;
}

void NNetModel::ResetAll( )
{

}

Shape const * NNetModel::GetShapeUnderPoint( MicroMeterPoint const pnt ) const
{
	for ( auto pShape : m_Shapes )
	{
		if ( pShape->IsPointInShape( pnt ) ) 
			return pShape;
	};

	return nullptr;
}
