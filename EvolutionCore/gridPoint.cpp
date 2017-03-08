// gridPoint.cpp :
//

#include "stdafx.h"
#include <limits.h>
#include <iostream>
#include <iomanip>
#include "gridPoint.h"

GridPoint const GridPoint::GP_NULL( SHRT_MIN, SHRT_MIN );
GridPoint const GridPoint::GRID_ORIGIN( 0,  0 );
GridPoint const GridPoint::GRID_SIZE( GridPoint::GRID_WIDTH, GridPoint::GRID_HEIGHT );

GridPointIntensity_Functor::GridPointIntensity_Functor( )
    : GridPoint_Functor( ),
    m_sIntensity( 0 )
{}

GridPointIntensity_Functor::GridPointIntensity_Functor( Grid * const pGrid )
    : GridPoint_Functor( pGrid ),
    m_sIntensity( 0 )
{}

std::wostream & operator << ( std::wostream & out, GridPoint const & gp )
{
//lint -e747  Significant prototype coercion with setw
    out << L" " << std::setw(3) << gp.x << L" " << std::setw(3) << gp.y << L" ";
    return out;
}
