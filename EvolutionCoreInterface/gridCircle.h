// gridCircle.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridPoint.h"
#include "gridRect.h"

class GridCircle
{
public:
    GridCircle( GridPoint const & gp, GRID_COORD const radius ) : m_gpCenter(gp), m_radius( radius ) {};

    ~GridCircle( ) { };

    GridPoint const & GetCenter( ) const { return m_gpCenter; };
    GRID_COORD        GetRadius( ) const { return m_radius; };

private:
    GridPoint  m_gpCenter;
    GRID_COORD m_radius;
};

GridCircle GetInscribedCircle( GridRect const & );

void Apply2Cone( GridPointIntensity_Functor *, GridCircle const &, short );

std::wostream & operator << ( std::wostream &, GridCircle const & );
