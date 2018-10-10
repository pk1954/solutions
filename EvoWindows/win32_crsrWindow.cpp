// win32_crsrWindow.cpp :
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "win32_crsrWindow.h"
#include "win32_focusPoint.h"

using namespace std;

CrsrWindow::CrsrWindow( ) :
    TextWindow( ),
    m_pFocusPoint( nullptr ),
    m_pCore( nullptr )
{ }

void CrsrWindow::Start
(
    HWND                  const hWndParent,
    FocusPoint          * const pFocusPoint,
    EvolutionCore const * const pCore
) 
{
    HWND hwnd = StartTextWindow( hWndParent, L"CrsrWindow", 100 );
    Move( 0, 300, 300, 150, TRUE );
    m_pCore = pCore;
    m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this );
}

void CrsrWindow::DoPaint( )
{
    printString( L"Position:" );

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
    printPercentage( m_pCore->GetFoodStock( gpFocus ), m_pCore->GetFertility( gpFocus ) );

    nextLine( L"MutRate:" );
    setHorizontalPos( 2 );
    printPercentage( m_pCore->GetMutRate( gpFocus ) );
    
	if ( m_pCore->IsDead( gpFocus ) )
        return;

    nextLine( L"Energy:" );
    setHorizontalPos( 4 );
    printPercentage( m_pCore->GetEnergy( gpFocus ), Config::GetConfigValueShort( Config::tId::stdCapacity ) );

	// Deactivated, see win32_focusPoint.cpp

    // nextLine( L"Lifespan:" );
    // printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
