// EvolutionCore.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "strategy.h"
#include "genome.h"
#include "grid_model.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"
#include "EvolutionCoreImpl.h"

static EvolutionCoreImpl * m_pCore = nullptr;

void EvolutionCore::InitClass( )
{
    Grid::InitClass(  );
}

EvolutionCore * EvolutionCore::CreateCore( )
{
    m_pCore = new EvolutionCoreImpl( );
    assert( m_pCore != nullptr );

    DefineCoreWrapperFunctions( m_pCore );

    return m_pCore;
}

void EvolutionCore::DeleteCore( )
{
    delete m_pCore;
}

unsigned long EvolutionCore::GetModelSize( )
{
	return sizeof( Grid );
}

unsigned int EvolutionCore::GetMaxPartnerMemory( )
{ 
	return StrategyData::GetMaxPartnerMemory( ); 
}

unsigned int EvolutionCore::GetNrInteractionsWithKnownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithKnownCulprit( ); 
}

unsigned int EvolutionCore::GetNrInteractionsWithUnknownCulprit( ) 
{ 
	return StrategyData::GetNrInteractionsWithUnknownCulprit( ); 
}

bool EvolutionCore::IsEnabled( tAction const action )
{
	return Genome::IsEnabled( action );
}

bool EvolutionCore::IsEnabled( tGeneType const gene )
{
	return Genome::IsEnabled( GetRelatedAction( gene ) );
}

