// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetPoint.h"
#include "NNetModel.h"

void NNetModel::InitClass
( 
	int                 const iNrOfNeighbors,
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
}

NNetModel::NNetModel( )
	: m_segment
	(
		NNetPoint(  20e3_NanoMeter,  20e3_NanoMeter ),
		NNetPoint( 800e3_NanoMeter, 400e3_NanoMeter ),
		NanoMeter( 40e3_NanoMeter )
	),
	m_pipeline
	(
		NNetPoint(  20e3_NanoMeter,  20e3_NanoMeter ),
		NNetPoint( 800e3_NanoMeter, 400e3_NanoMeter ),
		NanoMeter( 10e3_NanoMeter )
	),
	m_iCounter( 0 )
{
}

NNetModel * NNetModel::CreateModel( )
{
	return new NNetModel( );
}

void NNetModel::DestroyCore( NNetModel * pCore )
{
	delete pCore;
}

void NNetModel::Compute( )
{
	if ( ++ m_iCounter == 50 )
	{
		m_pipeline.Start();
		m_iCounter = 0;
	}
	else
	{
		m_pipeline.Step( );
	}
}

void NNetModel::ResetAll( )
{

}
