// EvoModelData.cpp
//

#include "stdafx.h"
#include "EvoGenerationCmd.h"
#include "EvolutionModelData.h"
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
	case tEvoCmd::reset:
		m_pWorkThread->WorkThread::ResetModel( );   // call layer 2
		break;

	case tEvoCmd::nextGen:
		m_pWorkThread->WorkThread::GenerationStep( );   // call layer 2
		break;

	case tEvoCmd::editDoEdit:
		m_pWorkThread->WorkThread::DoEdit( GridPoint( usParam ) );
		break;

	case tEvoCmd::editSetBrushShape:
	case tEvoCmd::editSetBrushSize:
	case tEvoCmd::editSetBrushIntensity:
	case tEvoCmd::editSetBrushMode:
		m_pWorkThread->WorkThread::ApplyEditorCommand( evoCmd, usParam );
		break;

	default:
		assert( false );
	}
}
