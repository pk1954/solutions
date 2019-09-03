// win32_crsrWindow.cpp :
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "win32_readBuffer.h"
#include "win32_focusPoint.h"
#include "win32_textBuffer.h"
#include "win32_crsrWindow.h"

CrsrWindow::CrsrWindow( ) :
    TextWindow( ),
    m_pFocusPoint( nullptr ),
    m_pReadBuffer( nullptr )
{}

CrsrWindow::~CrsrWindow( )
{
	m_pReadBuffer = nullptr;
	m_pFocusPoint = nullptr;
}

void CrsrWindow::Start
(
    HWND                        const hwndParent,
	ReadBuffer<EvolutionCore> * const pReadBuffer,
	FocusPoint                * const pFocusPoint
) 
{
    m_pReadBuffer = pReadBuffer;
    m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->RegisterFocusPointObserver( this );
    StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 415_PIXEL }, 
		L"CrsrWindow", 
		100, 
		TRUE,
		nullptr
	);
	m_pReadBuffer->RegisterObserver( this );
}

void CrsrWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( FALSE );
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

	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );

	ENERGY_UNITS enFoodStock = pCore->GetFoodStock( gpFocus );
	ENERGY_UNITS enFertility = pCore->GetFertility( gpFocus );
	ENERGY_UNITS enIndEnergy = pCore->GetEnergy( gpFocus );
	PERCENT      mutRate     = pCore->GetMutRate( gpFocus );
    bool         bIsAlive    = pCore->IsAlive( gpFocus );

	m_pReadBuffer->ReleaseReadBuffer( );

    textBuf.nextLine( L"Food:" );
    textBuf.setHorizontalPos( 3_TEXT_POSITION );
    textBuf.printPercentage( enFoodStock.GetValue(), enFertility.GetValue() );

    textBuf.nextLine( L"MutRate:" );
    textBuf.setHorizontalPos( 2_TEXT_POSITION );
    textBuf.printPercentage( mutRate.GetValue() );
    
	if ( bIsAlive )
	{
		textBuf.nextLine( L"Energy:" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printPercentage( enIndEnergy.GetValue(), Config::GetConfigValueShort( Config::tId::stdCapacity ) );
	}
	// Deactivated, see win32_focusPoint.cpp

    // nextLine( L"Lifespan:" );
    // printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
