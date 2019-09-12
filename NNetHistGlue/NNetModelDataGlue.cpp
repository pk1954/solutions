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


	default:
		assert( false );
	}
}
