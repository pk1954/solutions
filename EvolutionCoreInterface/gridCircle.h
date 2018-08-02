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

	void Apply2Cone( GridPointFuncShort const &, short const );

    GridPoint const & GetCenter( ) const { return m_gpCenter; };
    GRID_COORD        GetRadius( ) const { return m_radius; };

private:
    GridPoint  m_gpCenter;
    GRID_COORD m_radius;
};

GridCircle GetInscribedCircle( GridRect const & );

std::wostream & operator << ( std::wostream &, GridCircle const & );
