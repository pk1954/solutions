// EvolutionCore.cpp
//

#include "stdafx.h"
#include "strategy.h"
#include "genome.h"
#include "grid_model.h"
#include "gridNeighbor.h"
#include "EvolutionCore.h"
#include "EvolutionCoreImpl.h"
#include "EvolutionCoreWrappers.h"

EvolutionCore * EvolutionCore::InitClass
( 
	int                 const iNrOfNeighbors,
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
    Neighborhood::InitClass( iNrOfNeighbors );
    Grid::InitClass( iNrOfNeighbors );
	EvolutionCoreImpl::InitClass( pObservers, pEvent );
    EvolutionCore * pEvolutionCoreWork = EvolutionCore::CreateCore( );
	DefineCoreWrapperFunctions( pEvolutionCoreWork );
	return pEvolutionCoreWork;
}

EvolutionCore * EvolutionCore::CreateCore( )
{
    return new EvolutionCoreImpl( );
}

BYTES EvolutionCore::GetCoreSize() 
{ 
	return BYTES(sizeof(EvolutionCoreImpl)) + Grid::GetGridExtraSize(); 
};

unsigned int EvolutionCore::GetMaxPartnerMemory( )
{ 
	return StrategyData::GetMaxPartnerMemory( ).GetValue( ); 
}

unsigned int EvolutionCore::GetNrInteractionsWithKnownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithKnownCulprit( ); 
}

unsigned int EvolutionCore::GetNrInteractionsWithUnknownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithUnknownCulprit( ); 
}

bool EvolutionCore::IsEnabled( Action::Id const action )
{
	return Genome::IsEnabled( action );
}
