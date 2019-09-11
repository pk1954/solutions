// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"

void NNetModel::InitClass
( 
	int                 const iNrOfNeighbors,
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
}

NNetModel * NNetModel::CreateCore( )
{
	return new NNetModel( );
}

void NNetModel::DestroyCore( NNetModel * pCore )
{
	delete pCore;
}

void NNetModel::Compute( )
{

}

void NNetModel::ResetAll( )
{

}
