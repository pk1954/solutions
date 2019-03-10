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
    StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL_X, 300_PIXEL_Y, 300_PIXEL_X, 415_PIXEL_Y }, 
		L"CrsrWindow", 
		100, 
		TRUE 
	);
}

CrsrWindow::~CrsrWindow( )
{
	m_pCore = nullptr;
	m_pFocusPoint = nullptr;
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
    textBuf.setHorizontalPos( 3_TEXT_POSITION );
    textBuf.printPercentage( m_pCore->GetFoodStock( gpFocus ).GetValue(), m_pCore->GetFertility( gpFocus ).GetValue() );

    textBuf.nextLine( L"MutRate:" );
    textBuf.setHorizontalPos( 2_TEXT_POSITION );
    textBuf.printPercentage( m_pCore->GetMutRate( gpFocus ).GetValue() );
    
	if ( m_pCore->IsDead( gpFocus ) )
        return;

    textBuf.nextLine( L"Energy:" );
    textBuf.setHorizontalPos( 4_TEXT_POSITION );
    textBuf.printPercentage( m_pCore->GetEnergy( gpFocus ).GetValue(), Config::GetConfigValueShort( Config::tId::stdCapacity ) );

	// Deactivated, see win32_focusPoint.cpp

    // nextLine( L"Lifespan:" );
    // printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
