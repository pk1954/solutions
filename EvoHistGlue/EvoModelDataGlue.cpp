// EvoModelDataGlue.cpp
//

#include "stdafx.h"
#include "gridPoint24.h"
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

void EvoModelDataGlue::OnAppCommand( GenerationCmd const cmd  )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( cmd.GetCommand() );
	Int24   const param  = cmd.GetParam( );
	switch ( evoCmd )
	{
	case tEvoCmd::nextGen:
		m_pEvolutionCore->Compute( m_pEvolutionModelData );  // compute next generation
		break;

	case tEvoCmd::editDoEdit:
		m_pEvolutionModelData->ModelDoEdit( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::editSetPOI:
		m_pEvolutionModelData->SetPoi( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::reset:
		m_pEvolutionModelData->ResetAll( ); 
        break;

	case tEvoCmd::setSimulationMode:
        m_pEvolutionModelData->SetSimulationMode( static_cast<tBoolOp>( param.GetValue() ) );
        break;

	case tEvoCmd::editSetBrushMode:
        m_pEvolutionModelData->SetBrushMode( static_cast<tBrushMode>( param.GetValue() ) );
        break;

    case tEvoCmd::editSetBrushShape:
        m_pEvolutionModelData->SetBrushShape( static_cast<tShape>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushManipulator:
        m_pEvolutionModelData->SetBrushManipulator( static_cast<tManipulator>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushRadius:
        m_pEvolutionModelData->SetBrushRadius( static_cast<GRID_COORD>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushIntensity:
        m_pEvolutionModelData->SetBrushIntensity( static_cast<short>( param.GetValue() ) );
		break;

	default:
		assert( false );
	}
}
