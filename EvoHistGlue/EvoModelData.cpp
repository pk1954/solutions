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

void EvoModelData::OnAppCommand( int16_t const usCmd, int16_t const param  )
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
        m_pEvolutionModelData->SetBrushStrategy( static_cast<tBrushMode>( param ) );
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
