// gridNeighbor.h : 
//

#pragma once

#include <vector>
#include <functional>
#include "gridPoint.h"

class Grid;

using std::vector;

using NEIGHBORS = vector< GridPoint >;

class Neighborhood
{
public:
    static void InitClass( int const );

	static int GetNrOfNeighbors( ) 
	{ 
		return m_iNrOfNeighbors; 
	}

	Neighborhood( )	: m_neighbors( )
	{
		m_neighbors.reserve( m_iNrOfNeighbors );
	}

	void AddToList( GridPoint const & gp )
	{
		assert( static_cast<int>(m_neighbors.size()) < m_iNrOfNeighbors );
		m_neighbors.push_back( gp );
	}

	void RemoveFromList( int const iIndex )
	{
		assert( static_cast<int>(m_neighbors.size()) >= iIndex);
		m_neighbors.erase( m_neighbors.begin() + iIndex );
	}

	size_t GetLength( ) const 
	{ 
		return m_neighbors.size( ); 
	}

	void Clear( ) 
	{ 
		return m_neighbors.clear( ); 
	}

	GridPoint const GetElement( unsigned int const uiIndex  ) const 
	{ 
		return m_neighbors[uiIndex]; 
	}

    GridPoint const GetRandomElement( unsigned int const uiRandom ) const 
    {
        assert( m_neighbors.size( ) > 0 ); 
        //lint -e414   possible division by 0
        return m_neighbors[uiRandom % m_neighbors.size( )];
        //lint +e414 
    }

private:

	static int m_iNrOfNeighbors;

    NEIGHBORS m_neighbors;
};
