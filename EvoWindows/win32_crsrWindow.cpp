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
    GridPoint const gpFocus = m_pFocusPoint->GetGridPoint( );

    printString( L"Position:" );

    if ( !gpFocus.IsInGrid( ) )
    {
        printString( L"out of grid" );
        return;
    }

    printNumber( gpFocus.x );
    printNumber( gpFocus.y );

    // PART_FOOD

    nextLine( L"Food:" );
    setHorizontalPos( 3 );
    printPercentage( m_pModelWork->GetFoodStock( gpFocus ), m_pModelWork->GetFertility( gpFocus ) );

    // PART_MUTATION_RATE

    nextLine( L"MutRate:" );
    setHorizontalPos( 2 );
    printPercentage( m_pModelWork->GetMutationRate( gpFocus ) );
    
	// ++++++++++ EXPERIMENTAL ++++++++++++
/*
	PixelPoint pp = m_pGridWindow->GetRelativeCrsrPosition( );
    nextLine( L"pixel coord:" );
    printNumber( pp.x );
    printNumber( pp.y );

    nextLine( L"red coord:" );
    double dx = (double)pp.x;
	double dc = dx / 2 * sqrt( 5 );
	double dFieldSize;
	double dSize = dFieldSize * sqrt( 3 ) / 3;
	double q = pp.x * 2/3 / dSize;
    double r = (-pp.x / 3 + sqrt(3)/3 * pp.y) / dSize;

    printNumber( floor(dc + 0.5) );
    printNumber( pp.x );
*/
	if ( m_pModelWork->IsDead( gpFocus ) )
        return;

    // PART_ENERGY

    nextLine( L"Energy:" );
    setHorizontalPos( 4 );
    printPercentage( m_pModelWork->GetEnergy( gpFocus ), EvolutionCore::GetStdCapacity( ) );
    
    // PART_LIFESPAN

    nextLine( L"Lifespan:" );
    printSpan( m_pFocusPoint->GetGenBirth( ).GetLong( ), m_pFocusPoint->GetGenDeath( ).GetLong( ) );
}
