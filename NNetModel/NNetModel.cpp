// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "NNetModel.h"

using namespace std::chrono;

NNetModel::NNetModel( )
  : m_timeStamp( microseconds( 0 ) ),
	m_Shapes( ),
	m_shapeHighlighted( NO_SHAPE )
{
	InputNeuron * pNeuron1  = new InputNeuron( MicroMeterPoint( 400.0_MicroMeter,  200.0_MicroMeter ) );
	Knot        * pKnot     = new Knot( MicroMeterPoint( 400.0_MicroMeter, 1400.0_MicroMeter ) );
	Pipeline    * pPipeline = new Pipeline( 0.1_meterPerSec );

	AddShape( * pNeuron1 );
	AddShape( * pKnot );
	AddShape( * pPipeline );

	pNeuron1->AddOutgoing( pPipeline );
	pKnot->AddIncomming  ( pPipeline );
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
