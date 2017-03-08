// gridNeighbor.h : 
//

#pragma once

#include <array>
#include "gridPoint.h"

class NeighborhoodIterator
{
public:
    static int const NR_OF_NEIGHBOURS = 8;

    explicit          NeighborhoodIterator( GridPoint const & );

    bool              NextNeighbor( );
    GridPoint const & GetNeighbor( ) const { return m_gpNeighbor; };

private:
    GridPoint calcNeighbor( ) const          // faster, but more complicated
    { 
        GridPoint gpNeighbor = m_gpCenter + m_gpDelta;

        if ( gpNeighbor.x < 0 )
            gpNeighbor.x += GridPoint::GRID_SIZE.x;
        else if ( gpNeighbor.x >= GridPoint::GRID_SIZE.x )
            gpNeighbor.x -= GridPoint::GRID_SIZE.x;

        if ( gpNeighbor.y < 0 )
            gpNeighbor.y += GridPoint::GRID_SIZE.y;
        else if ( gpNeighbor.y >= GridPoint::GRID_SIZE.y )
            gpNeighbor.y -= GridPoint::GRID_SIZE.y;

        return gpNeighbor; 
    }; 

    GridPoint m_gpCenter;
    GridPoint m_gpDelta;
    GridPoint m_gpNeighbor;

    static long const WEST   = -1;
    static long const EAST   =  1;
    static long const NORTH  = -1;
    static long const SOUTH  =  1;
    static long const CENTER =  0;

    static GridPoint const GP_NORTH_WEST;
};

typedef std::array< GridPoint, NeighborhoodIterator::NR_OF_NEIGHBOURS > ARRAY_OF_NEIGHBORS;

//lint -esym(1565,NeighborList::m_list)   // not assigned by initializer function
class NeighborList
{
public:
    //lint -sem(NeighborList::ClearList,initializer)
    NeighborList( ) { ClearList(); };

    void AddToList( GridPoint const & gp ) { m_list[ m_uiLength++ ] = gp; }
    void ClearList( ) { m_uiLength = 0; }
    unsigned int      GetLength       ( )                             const { return m_uiLength; }
    GridPoint const & GetElement      ( unsigned int const uiIndex  ) const { return m_list[uiIndex]; }
    GridPoint const & GetRandomElement( unsigned int const uiRandom ) const 
    {
        assert( m_uiLength > 0 ); 
        //lint -e414   possible division by 0
        return m_list[uiRandom % m_uiLength];
        //lint +e414 
    }

private:
    std::array< GridPoint, NeighborhoodIterator::NR_OF_NEIGHBOURS > m_list;
    unsigned int m_uiLength;
};

class GridPointNeighbor_Functor
{
public:
    explicit GridPointNeighbor_Functor( GridPoint const & gp ) : m_gpCenter( gp ) { } 
    virtual ~GridPointNeighbor_Functor() { };
 
    GridPoint const & GetCenter( ) const { return m_gpCenter; }

    virtual bool operator() ( GridPoint const &, GridPoint const & ) const = 0;

private:
    GridPoint const m_gpCenter;
};

void Apply2AllNeighbors( GridPointNeighbor_Functor const & );

