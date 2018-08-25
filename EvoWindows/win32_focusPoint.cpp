// win32_focusPoint.cpp
//

#include "stdafx.h"
#include "EvoHistorySysGlue.h"
#include "EvolutionModelData.h"
#include "win32_viewCollection.h"
#include "win32_focusPoint.h"

FocusPoint::FocusPoint( ) :
    m_pEvoHistGlue( nullptr ),
    m_pModelWork( nullptr ),
    m_genBirth( - 1 ),
    m_genDeath( - 1 ),
    m_gp( )
{ 
	m_pViewCol = new ViewCollection( );
};

void FocusPoint::Start
( 
    EvoHistorySysGlue  * pEvoHistGlue,
    EvolutionModelData * pModel
)
{
    assert( pModel != nullptr );
    m_pEvoHistGlue = pEvoHistGlue;
    m_pModelWork   = pModel;
}

FocusPoint::~FocusPoint( ) 
{ 
	delete m_pViewCol;
};

GridPoint const FocusPoint::GetGridPoint( ) const 
{ 
	return m_gp; 
}

HIST_GENERATION const FocusPoint::GetGenBirth( ) const 
{ 
	return m_genBirth; 
}

HIST_GENERATION const FocusPoint::GetGenDeath( ) const 
{ 
	return m_genDeath; 
}

BOOL const FocusPoint::IsInGrid( ) const 
{ 
	return m_gp.IsInGrid( ); 
}

BOOL const FocusPoint::IsAlive( ) const 
{ 
	return m_pModelWork->IsAlive( m_gp ); 
}

BOOL const FocusPoint::IsDead( ) const 
{ 
	return m_pModelWork->IsDead( m_gp ); 
}

BOOL const FocusPoint::IsDefined( ) const 
{ 
	return m_pModelWork->IsDefined( m_gp ); 
}

void FocusPoint::SetFocusPoint( GridPoint const gpNew )
{
    if ( gpNew != m_gp )
    {
        m_gp = gpNew;
        if ( IsInGrid( ) )
        {
            if ( m_pEvoHistGlue != nullptr )
            {
                IndId id = m_pModelWork->GetId( m_gp );
                m_genBirth = m_pEvoHistGlue->GetFirstGenOfIndividual( id );
                m_genDeath = m_pEvoHistGlue->GetLastGenOfIndividual ( id ) + 1;
            }
        }
        m_pViewCol->NotifyObservers( );
    }
}

void FocusPoint::AttachFocusPointObserver( RootWindow const * pRootWindow, INT const iMilliSecs )
{
    m_pViewCol->AttachObserver( pRootWindow, iMilliSecs );
}
