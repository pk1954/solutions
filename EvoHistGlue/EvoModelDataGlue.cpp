// EvoModelDataGlue.cpp
//

#include "stdafx.h"
#include "gridPoint24.h"
#include "EvoGenerationCmd.h"
#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"
#include "win32_stopwatch.h"

static Stopwatch stopwatch;

EvoModelDataGlue::~EvoModelDataGlue( )
{
    delete m_pEvolutionCore;
}

GridPoint EvoModelDataGlue::FindGridPoint( IndId const & id ) const
{ 
    return m_pEvolutionCore->FindGridPoint( id );
}

void EvoModelDataGlue::CopyFrom( ModelData const * const src )
{
//	stopwatch.Start();
	EvoModelDataGlue const * const evoSrc = static_cast< EvoModelDataGlue const * const >( src );
    m_pEvolutionCore->CopyEvolutionCoreData( evoSrc->m_pEvolutionCore );
//	stopwatch.Stop( L"Copy model" );
}

	void EvoModelDataGlue::OnAppCommand( GenerationCmd const cmd )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( cmd.GetCommand() );
	Int24   const param  = cmd.GetParam( );
	switch ( evoCmd )
	{
	case tEvoCmd::nextGen:

		stopwatch.Start();
		m_pEvolutionCore->Compute( );  // compute next generation
		stopwatch.Stop( L"Compute" );
		break;

	case tEvoCmd::editDoEdit:
		m_pEvolutionCore->ModelDoEdit( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::editSetPOI:
		m_pEvolutionCore->SetPoi( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::reset:
		m_pEvolutionCore->ResetAll( ); 
        break;

	case tEvoCmd::setSimulationMode:
        m_pEvolutionCore->SetSimulationMode( static_cast<tBoolOp>( param.GetValue() ) );
        break;

	case tEvoCmd::editSetBrushMode:
        m_pEvolutionCore->SetBrushMode( static_cast<tBrushMode>( param.GetValue() ) );
        break;

    case tEvoCmd::editSetBrushShape:
        m_pEvolutionCore->SetBrushShape( static_cast<tShape>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushManipulator:
        m_pEvolutionCore->SetBrushManipulator( static_cast<tManipulator>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushRadius:
        m_pEvolutionCore->SetBrushRadius( static_cast<GRID_COORD>( param.GetValue() ) );
		break;

    case tEvoCmd::editSetBrushIntensity:
        m_pEvolutionCore->SetBrushIntensity( static_cast<short>( param.GetValue() ) );
		break;

	default:
		assert( false );
	}
}
