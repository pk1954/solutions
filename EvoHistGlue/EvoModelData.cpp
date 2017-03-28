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

void EvoModelData::OnAppCommand( unsigned short usCmd, short sParam  )
{
	tEvoCmd const evoCmd = static_cast<tEvoCmd>( usCmd );
	switch ( evoCmd )
	{
	case tEvoCmd::editSetXvalue:
		m_gpEdit.x = sParam;
		break;

	case tEvoCmd::editSetYvalue:
		m_gpEdit.y = sParam;
		m_pWorkThread->WorkThread::DoEdit( m_gpEdit );
		break;

	case tEvoCmd::editSetBrushShape:
	case tEvoCmd::editSetBrushSize:
	case tEvoCmd::editSetBrushIntensity:
	case tEvoCmd::editSetBrushMode:
		m_pWorkThread->WorkThread::ApplyEditorCommand( evoCmd, sParam );
		break;

	default:
		assert( false );
	}
}
