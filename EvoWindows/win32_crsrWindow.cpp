// win32_crsrWindow.cpp :
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "win32_focusPoint.h"
#include "win32_textBuffer.h"
#include "win32_crsrWindow.h"

CrsrWindow::CrsrWindow( ) :
    TextWindow( ),
    m_pFocusPoint( nullptr ),
    m_pCore( nullptr )
{ }

void CrsrWindow::Start
(
    HWND                  const hwndParent,
    FocusPoint          * const pFocusPoint,
    EvolutionCore const * const pCore
) 
{
    m_pCore = pCore;
    m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this );
    StartTextWindow( hwndParent, 0, 300, 300, 115, L"CrsrWindow", 100, TRUE );
}

void CrsrWindow::DoPaint( TextBuffer & textBuf )
{
    textBuf.printString( L"Position:" );

    GridPoint const gpFocus = m_pFocusPoint->GetGridPoint( );

    if ( ! IsInGrid( gpFocus ) )
    {
        textBuf.printString( L"out of grid" );
        return;
    }

    textBuf.printNumber( gpFocus.GetXvalue() );
    textBuf.printNumber( gpFocus.GetYvalue() );

    textBuf.nextLine( L"Food:" );
    textBuf.setHorizontalPos( 3 );
    textBuf.printPercentage( m_pCore->GetFoodStock( gpFocus ), m_pCore->GetFertility( gpFocus ) );

    textBuf.nextLine( L"MutRate:" );
    textBuf.setHorizontalPos( 2 );
    textBuf.printPercentage( m_pCore->GetMutRate( gpFocus ) );
    
	if ( m_pCore->IsDead( gpFocus ) )
        return;

    textBuf.nextLine( L"Energy:" );
    textBuf.setHorizontalPos( 4 );
    textBuf.printPercentage( m_pCore->GetEnergy( gpFocus ), Config::GetConfigValueShort( Config::tId::stdCapacity ) );

	// Deactivated, see win32_focusPoint.cpp

    // nextLine( L"Lifespan:" );
    // printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
