// win32_crsrWindow.cpp :
//

#include "stdafx.h"
#include "config.h"
#include "EvoModelData.h"
#include "EvolutionCore.h"
#include "win32_crsrWindow.h"
#include "win32_focusPoint.h"

// ++++++++++ EXPERIMENTAL ++++++++++++

#include "win32_gridWindow.h" 
#include "pixelPoint.h"
#include "win32_util.h"

// ++++++++++ end EXPERIMENTAL ++++++++++++

using namespace std;

CrsrWindow::CrsrWindow( ) :
    TextWindow( ),
    m_pFocusPoint( nullptr ),
    m_pModelWork( nullptr )
{ }

void CrsrWindow::Start
(
    HWND               const         hWndParent,
    FocusPoint       * const         pFocusPoint,
    EvolutionModelData const * const pModel,
	GridWindow         const * const pGridWindow   // ++++++++++ EXPERIMENTAL ++++++++++++
) 
{
    HWND hWnd = StartTextWindow( hWndParent, L"CrsrWindow", 100 );
    Move( 0, 300, 300, 150, TRUE );
    m_pModelWork = pModel;
    m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this, 0 );
	m_pGridWindow = pGridWindow;      // ++++++++++ EXPERIMENTAL ++++++++++++
}

void CrsrWindow::DoPaint( )
{
    printString( L"pixel coord:" );

	PixelPoint pp = m_pGridWindow->GetRelativeCrsrPosition( );
    printNumber( pp.x );
    printNumber( pp.y );

    nextLine( L"Position:" );

    GridPoint const gpFocus = m_pFocusPoint->GetGridPoint( );

    if ( !gpFocus.IsInGrid( ) )
    {
        printString( L"out of grid" );
        return;
    }

    printNumber( gpFocus.x );
    printNumber( gpFocus.y );

    nextLine( L"Food:" );
    setHorizontalPos( 3 );
    printPercentage( m_pModelWork->GetFoodStock( gpFocus ), m_pModelWork->GetFertility( gpFocus ) );

    nextLine( L"MutRate:" );
    setHorizontalPos( 2 );
    printPercentage( m_pModelWork->GetMutationRate( gpFocus ) );
    
	if ( m_pModelWork->IsDead( gpFocus ) )
        return;

    nextLine( L"Energy:" );
    setHorizontalPos( 4 );
    printPercentage( m_pModelWork->GetEnergy( gpFocus ), EvolutionCore::GetStdCapacity( ) );
    
    nextLine( L"Lifespan:" );
    printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
