// gridRect.cpp :
//

#include "stdafx.h"
#include "limits.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "gridRect.h"

using namespace std;

GridRect const GridRect::GRID_RECT_FULL ( GridPoint::GRID_ORIGIN, GridPoint::GRID_SIZE );

void GridRect::ClipToGrid( )
{
    if ( GetTop() < GRID_RECT_FULL.GetTop() )
         m_gpExtension.y = m_gpCenter.y - GRID_RECT_FULL.GetTop();

    if ( GetBottom() > GRID_RECT_FULL.GetBottom() )
         m_gpExtension.y = GRID_RECT_FULL.GetBottom() - m_gpCenter.y;

    if ( GetLeft() < GRID_RECT_FULL.GetLeft() )
         m_gpExtension.x = m_gpCenter.x - GRID_RECT_FULL.GetLeft();

    if ( GetRight() > GRID_RECT_FULL.GetRight() )
         m_gpExtension.x = GRID_RECT_FULL.GetRight() - m_gpCenter.x;

    if ( (m_gpExtension.x < 0) || (m_gpExtension.y < 0) )
        Reset();
}
 
void GridRect::Apply2Shape( GridPointFuncShort const & func, short const s ) const
{
    GRID_COORD const gcLeft   = max( GetLeft  (), GridRect::GRID_RECT_FULL.GetLeft  () );
    GRID_COORD const gcTop    = max( GetTop   (), GridRect::GRID_RECT_FULL.GetTop   () );
    GRID_COORD const gcRight  = min( GetRight (), GridRect::GRID_RECT_FULL.GetRight () );
    GRID_COORD const gcBottom = min( GetBottom(), GridRect::GRID_RECT_FULL.GetBottom() );

    GridPoint gp;
    for ( gp.y = gcTop; gp.y <= gcBottom; ++gp.y )
        for ( gp.x = gcLeft; gp.x <= gcRight; ++gp.x )
            func( gp, s );
}

void GridRect::Apply2Rect( GridPointFunc const & func ) const
{
    GRID_COORD const gcLeft   = max( GetLeft  (), GridRect::GRID_RECT_FULL.GetLeft  () );
    GRID_COORD const gcTop    = max( GetTop   (), GridRect::GRID_RECT_FULL.GetTop   () );
    GRID_COORD const gcRight  = min( GetRight (), GridRect::GRID_RECT_FULL.GetRight () );
    GRID_COORD const gcBottom = min( GetBottom(), GridRect::GRID_RECT_FULL.GetBottom() );

    GridPoint gp;
    for ( gp.y = gcTop; gp.y <= gcBottom; ++gp.y )
        for ( gp.x = gcLeft; gp.x <= gcRight; ++gp.x )
            func( gp );
}

void Apply2Grid( GridPointFunc const & func )
{
    GridRect::GRID_RECT_FULL.Apply2Rect( func );
}

std::wostream & operator << ( std::wostream & out, GridRect const & rect )
{
    out << rect.GetStartPoint() << L' ' << rect.GetEndPoint();
    return out;
}
