// win32_crsrWindow.cpp :
//

#include "stdafx.h"
#include "config.h"
#include "EvoModelData.h"
#include "win32_crsrWindow.h"
#include "win32_focusPoint.h"

using namespace std;

CrsrWindow::CrsrWindow( ) :
    TextWindow( ),
    m_pFocusPoint( nullptr ),
    m_pModelWork( nullptr )
{ }

void CrsrWindow::Start
(
    HWND              const hWndParent,
    FocusPoint      * const pFocusPoint,
    EvolutionModelData const * const pModel
) 
{
    HWND hWnd = StartTextWindow( hWndParent, L"CrsrWindow", 100 );
    Move( 0, 300, 300, 150, TRUE );
    m_pModelWork = pModel;
    m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this, 0 );
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
