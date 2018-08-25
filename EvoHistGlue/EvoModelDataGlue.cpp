// EvoModelDataGlue.cpp
//

#include "stdafx.h"
#include "EvoGenerationCmd.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"

EvoModelDataGlue::~EvoModelDataGlue( )
{
    delete m_pEvolutionModelData;
}

GridPoint EvoModelDataGlue::FindGridPoint( IndId const & id ) const
{ 
    return m_pEvolutionModelData->FindGridPoint( id );
}

void EvoModelDataGlue::CopyModelData( ModelData const * const src )
{
	EvoModelDataGlue const * const evoSrc = static_cast< EvoModelDataGlue const * const >( src );
    m_pEvolutionModelData->CopyEvolutionModelData( evoSrc->m_pEvolutionModelData );
}

void EvoModelDataGlue::OnAppCommand( tGenCmd const usCmd, int16_t const param  )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( usCmd );
	switch ( evoCmd )
	{
	case tEvoCmd::nextGen:
		m_pEvolutionCore->Compute( m_pEvolutionModelData );  // compute next generation
		break;

	case tEvoCmd::editDoEdit:
		m_pEvolutionModelData->ModelDoEdit( GridPoint( static_cast<unsigned short>( param ) )  );
		break;

	case tEvoCmd::editSetPOI:
		m_pEvolutionModelData->SetPoi( GridPoint( static_cast<unsigned short>( param )  )  );
		break;

	case tEvoCmd::reset:
		m_pEvolutionModelData->ResetAll( ); 
        break;

	case tEvoCmd::setSimulationMode:
        m_pEvolutionModelData->SetSimulationMode( static_cast<tBoolOp>( param ) );
        break;

	case tEvoCmd::editSetBrushMode:
        m_pEvolutionModelData->SetBrushMode( static_cast<tBrushMode>( param ) );
        break;

    case tEvoCmd::editSetBrushShape:
        m_pEvolutionModelData->SetBrushShape( static_cast<tShape>( param ) );
		break;

    case tEvoCmd::editSetBrushOperator:
        m_pEvolutionModelData->SetBrushOperator( static_cast<tOperator>( param ) );
		break;

    case tEvoCmd::editSetBrushSize:
		assert( param <= MAX_GRID_COORD );
        m_pEvolutionModelData->SetBrushSize( static_cast<GRID_COORD>( param ) );
		break;

    case tEvoCmd::editSetBrushIntensity:
        m_pEvolutionModelData->SetBrushIntensity( static_cast<short>( param ) );
		break;

	default:
		assert( false );
	}
}
