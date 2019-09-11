// NNetModelDataGlue.cpp
//
// NNetHistGlue

#include "stdafx.h"
#include "NNetGenerationCmd.h"
#include "NNetModel.h"
#include "NNetModelDataGlue.h"

Stopwatch NNetModelDataGlue::stopwatch;

void NNetModelDataGlue::OnAppCommand( GenerationCmd const cmd )
{
	tNNetCmd const nnetCmd = static_cast<tNNetCmd>( cmd.GetCommand() );
	Int24    const param   = cmd.GetParam( );
	switch ( nnetCmd )
	{
	case tNNetCmd::nextGen:

//		stopwatch.Start();
		m_pNNetModel->Compute( );  // compute next generation
//		stopwatch.Stop( L"Compute" );
		break;

	case tNNetCmd::reset:
		m_pNNetModel->ResetAll( ); 
        break;

	default:
		assert( false );
	}
}
