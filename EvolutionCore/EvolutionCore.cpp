// EvolutionCore.cpp
//

#include "stdafx.h"
#include "assert.h"
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
