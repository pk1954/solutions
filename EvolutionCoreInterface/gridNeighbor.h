// gridNeighbor.h : 
//

#pragma once

#include <array>
#include "gridPoint.h"

static int const NR_OF_NEIGHBORS = 8;

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
    std::array< GridPoint, NR_OF_NEIGHBORS > m_list;
    unsigned int m_uiLength;
};

class GridPointNeighbor_Functor
{
public:
    explicit GridPointNeighbor_Functor( GridPoint const & gp ) : m_gpCenter( gp ) { } 
    virtual ~GridPointNeighbor_Functor() { };
 
    GridPoint const & GetCenter( ) const { return m_gpCenter; }

    virtual bool operator() ( GridPoint const & ) const = 0;

private:
    GridPoint const m_gpCenter;
};

void Apply2AllNeighbors( GridPointNeighbor_Functor const & );
