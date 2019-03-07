// EvolutionCore.cpp
//

#include "stdafx.h"
#include "strategy.h"
#include "genome.h"
#include "grid_model.h"
#include "EvolutionCore.h"
#include "EvolutionCoreImpl.h"
#include "EvolutionCoreWrappers.h"

EvolutionCore * EvolutionCore::InitClass
( 
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
    Grid::InitClass( );
	EvolutionCoreImpl::InitClass( pObservers, pEvent );
    EvolutionCore * pEvolutionCore = EvolutionCore::CreateCore( );
	DefineCoreWrapperFunctions( pEvolutionCore );
	return pEvolutionCore;
}

EvolutionCore * EvolutionCore::CreateCore( )
{
    return new EvolutionCoreImpl( );
}

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

bool EvolutionCore::IsEnabled( GeneType::Id const gene )
{
	return Genome::IsEnabled( GetRelatedAction( gene ) );
}

