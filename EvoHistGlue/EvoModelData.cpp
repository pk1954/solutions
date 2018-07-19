// EvoModelData.cpp
//

#include "stdafx.h"
#include "EvoGenerationCmd.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvoModelData.h"

EvoModelData::~EvoModelData( )
{
    delete m_pEvolutionModelData;
}

GridPoint EvoModelData::FindGridPoint( IndId const & id ) const
{ 
    return m_pEvolutionModelData->FindGridPoint( id );
}

void EvoModelData::CopyModelData( ModelData const * const src )
{
	EvoModelData const * const evoSrc = static_cast< EvoModelData const * const >( src );
    m_pEvolutionModelData->CopyEvolutionModelData( evoSrc->m_pEvolutionModelData );
}

void EvoModelData::OnAppCommand( unsigned short const usCmd, unsigned short const usParam  )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( usCmd );
	switch ( evoCmd )
	{
	case tEvoCmd::nextGen:
		m_pEvolutionCore->Compute( m_pEvolutionModelData );  // compute next generation
		break;

	case tEvoCmd::editDoEdit:
		m_pEvolutionModelData->ModelDoEdit( GridPoint( usParam )  );
		break;

	case tEvoCmd::editSetPOI:
		m_pEvolutionModelData->SetPoi( GridPoint( usParam )  );
		break;

	case tEvoCmd::reset:
		m_pEvolutionModelData->ResetAll( ); 
        break;

	case tEvoCmd::editSetBrushMode:
        m_pEvolutionModelData->SetBrushStrategy( static_cast<tBrushMode>( usParam ) );
        break;

    case tEvoCmd::editSetBrushShape:
        m_pEvolutionModelData->SetBrushShape( static_cast<tShape>( usParam ) );
		break;

    case tEvoCmd::editSetBrushSize:
		assert( usParam <= MAX_GRID_COORD );
        m_pEvolutionModelData->SetBrushSize( static_cast<GRID_COORD>( usParam ) );
		break;

    case tEvoCmd::editSetBrushIntensity:
        m_pEvolutionModelData->SetBrushIntensity( usParam );
		break;

	default:
		assert( false );
	}
}
