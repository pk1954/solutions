// EvoModelData.cpp
//

#include "stdafx.h"
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
	switch ( static_cast<tEvoCmd>( usCmd ))
	{
	case tEvoCmd::editSetXvalue:
		m_gpEdit.x = sParam;
		break;

	case tEvoCmd::editSetYvalue:
		m_gpEdit.y = sParam;
		m_pEvolutionModelData->ModelDoEdit( m_gpEdit );
		break;

	case tEvoCmd::editSetBrushShape:
		m_pEvolutionModelData->SetBrushShape( static_cast<tShape>( sParam ) );
		break;

	case tEvoCmd::editSetBrushSize:
		m_pEvolutionModelData->SetBrushSize( sParam );
		break;

	case tEvoCmd::editSetBrushIntensity:
		m_pEvolutionModelData->SetBrushIntensity( sParam );
		break;

	case tEvoCmd::editSetBrushMode:
		m_pEvolutionModelData->SetBrushStrategy( static_cast<tBrushMode>( sParam ) );
		break;

	default:
		assert( false );
	}
}
