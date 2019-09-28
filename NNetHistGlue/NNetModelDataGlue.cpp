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
	NNetGenerationCmd::Id const nnetCmd = static_cast<NNetGenerationCmd::Id>( cmd.GetCommand() );
	Int24    const param   = cmd.GetParam( );
	switch ( nnetCmd )
	{
	case NNetGenerationCmd::Id::editFirst:
		break;

	default:
		assert( false );
	}
}
