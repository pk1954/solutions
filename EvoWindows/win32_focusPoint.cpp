// win32_focusPoint.cpp
//

#include "stdafx.h"
#include "EvoHistorySysGlue.h"
#include "win32_viewCollection.h"
#include "win32_focusPoint.h"

FocusPoint::FocusPoint( ) :
    m_pEvoHistGlue( nullptr ),
    m_gp( GP_NULL )
{ };

void FocusPoint::Start
( 
    EvoHistorySysGlue * pEvoHistGlue
)
{
    assert( pCore != nullptr );
    m_pEvoHistGlue = pEvoHistGlue;
}

void FocusPoint::SetFocusPoint( GridPoint const gpNew )
{
    if ( gpNew != m_gp )
    {
        m_gp = gpNew;
        m_ViewCollection.Notify( false );
    }
}
