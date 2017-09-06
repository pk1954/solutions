// gridRect.cpp :
//

#include "stdafx.h"
#include "limits.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "gridRect.h"

using namespace std;

GridRect const GridRect::GRID_RECT_FULL ( 0, 0, GridPoint::GRID_WIDTH - 1, GridPoint::GRID_HEIGHT - 1 );
GridRect const GridRect::GRID_RECT_EMPTY( 0, 0, 0, 0 );

void GridRect::Move( GridPoint const & gpDiff )
{
    m_lLeft   += gpDiff.x;
    m_lRight  += gpDiff.x;
    m_lTop    += gpDiff.y;
    m_lBottom += gpDiff.y;
}

void GridRect::SetCenter( GridPoint const & gpCenter )
{
    GridPoint gpSize = GetSize( );
    m_lLeft   = gpCenter.x - gpSize.x / 2;
    m_lRight  = gpCenter.x + gpSize.x / 2;
    m_lTop    = gpCenter.y - gpSize.y / 2;
    m_lBottom = gpCenter.y + gpSize.y / 2;
}

void GridRect::SetSize( GridPoint const & gpSize )
{
    GridPoint gpCenter = GetCenter( );
    m_lLeft   = gpCenter.x - gpSize.x / 2;
    m_lRight  = gpCenter.x + gpSize.x / 2;
    m_lTop    = gpCenter.y - gpSize.y / 2;
    m_lBottom = gpCenter.y + gpSize.y / 2;
}

void GridRect::ClipToGrid( )
{
    if ( m_lTop < GRID_RECT_FULL.m_lTop )
         m_lTop = GRID_RECT_FULL.m_lTop;

    if ( m_lBottom > GRID_RECT_FULL.m_lBottom )
         m_lBottom = GRID_RECT_FULL.m_lBottom;

    if ( m_lLeft < GRID_RECT_FULL.m_lLeft )
         m_lLeft = GRID_RECT_FULL.m_lLeft;

    if ( m_lRight > GRID_RECT_FULL.m_lRight )
         m_lRight = GRID_RECT_FULL.m_lRight;

    if ( (m_lTop > m_lBottom) || (m_lLeft > m_lRight) )
        *this = GRID_RECT_EMPTY;
}
 
void Apply2Rect( GridPoint_Functor * const pgpf, GridRect const & rect )
{
    GRID_COORD const gcLeft   = max( rect.GetLeft  (), GridRect::GRID_RECT_FULL.GetLeft  () );
    GRID_COORD const gcTop    = max( rect.GetTop   (), GridRect::GRID_RECT_FULL.GetTop   () );
    GRID_COORD const gcRight  = min( rect.GetRight (), GridRect::GRID_RECT_FULL.GetRight () );
    GRID_COORD const gcBottom = min( rect.GetBottom(), GridRect::GRID_RECT_FULL.GetBottom() );

    GridPoint gp;
    for ( gp.y = gcTop; gp.y <= gcBottom; ++gp.y )
        for ( gp.x = gcLeft; gp.x <= gcRight; ++gp.x )
            ( * pgpf )( gp );
}

void Apply2Grid( GridPoint_Functor * const pgpf )
{
    Apply2Rect( pgpf, GridRect::GRID_RECT_FULL );
}

std::wostream & operator << ( std::wostream & out, GridRect const & rect )
{
    out << rect.GetStartPoint() << L' ' << rect.GetEndPoint();
    return out;
}
