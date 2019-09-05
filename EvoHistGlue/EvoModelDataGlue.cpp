// EvoModelDataGlue.cpp
//
// EvoHistGlue

#include "stdafx.h"
#include "gridPOI.h"
#include "gridPoint24.h"
#include "EvoGenerationCmd.h"
#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"

Stopwatch EvoModelDataGlue::stopwatch;

void EvoModelDataGlue::OnAppCommand( GenerationCmd const cmd )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( cmd.GetCommand() );
	Int24   const param  = cmd.GetParam( );
	switch ( evoCmd )
	{
	case tEvoCmd::nextGen:

//		stopwatch.Start();
		m_pEvolutionCore->Compute( );  // compute next generation
//		stopwatch.Stop( L"Compute" );
		break;

	case tEvoCmd::editDoEdit:
		m_pEvolutionCore->ModelDoEdit( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::editSetPOI:
		GridPOI::SetPoi( GridPoint24::Unpack( param ) );
		break;

	case tEvoCmd::reset:
		m_pEvolutionCore->ResetAll( ); 
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
        m_pEvolutionCore->SetBrushIntensity( static_cast<PERCENT>( param.GetValue() ) );
		break;

	default:
		assert( false );
	}
}
