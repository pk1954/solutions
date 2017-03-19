// EvolutionCore.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "grid_model.h"
#include "wrappers.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvolutionCoreImpl.h"

static EvolutionCoreImpl * m_pCore = nullptr;

EvolutionModelData * EvolutionModelData::CreateModelData( ) 
{ 
    return new ModelDataImpl( ); 
}

void EvolutionCore::InitClass( )
{
    Grid::InitClass( );
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

int EvolutionCore::GetStdCapacity( ) 
{ 
    return  EvolutionCoreImpl::GetStdCapacity( );
};

