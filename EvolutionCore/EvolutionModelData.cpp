// EvolutionModelData.cpp
//

#include "stdafx.h"
#include "strategy.h"
#include "genome.h"
#include "grid_model.h"
#include "EvolutionModelData.h"
#include "EvolutionModelDataImpl.h"

EvolutionModelData * EvolutionModelData::CreateModelData( ) 
{ 
    return new EvolutionModelDataImpl( ); 
}

unsigned long EvolutionModelData::GetModelSize( )
{
	return sizeof( Grid );
}

unsigned int EvolutionModelData::GetMaxPartnerMemory( )
{ 
	return StrategyData::GetMaxPartnerMemory( ); 
}

unsigned int EvolutionModelData::GetNrInteractionsWithKnownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithKnownCulprit( ); 
}

unsigned int EvolutionModelData::GetNrInteractionsWithUnknownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithUnknownCulprit( ); 
}

bool EvolutionModelData::IsEnabled( tAction const action )
{
	return Genome::IsEnabled( action );
}

bool EvolutionModelData::IsEnabled( tGeneType const gene )
{
	return Genome::IsEnabled( GetRelatedAction( gene ) );
}
