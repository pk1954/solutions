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
    GridCircle( GridPoint const & gp, short const sRad ) : m_gpCenter(gp), m_sRadius(sRad) {};

    ~GridCircle( ) { };

    GridPoint const & GetCenter( ) const { return m_gpCenter; };
    short             GetRadius( ) const { return m_sRadius; };

private:
    GridPoint m_gpCenter;
    short     m_sRadius;
};

GridCircle GetInscribedCircle( GridRect const & );

void Apply2Cone( GridPointIntensity_Functor *, GridCircle const &, short );

std::wostream & operator << ( std::wostream &, GridCircle const & );
