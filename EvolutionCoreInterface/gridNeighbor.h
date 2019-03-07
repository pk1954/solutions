// gridNeighbor.h : 
//

#pragma once

#include <vector>
#include <functional>
#include "gridPoint.h"
#include "GridDimensions.h"

using std::vector;

class Neighborhood
{
public:
    static void InitClass( int const );

	static void Apply2All( GridPoint const gpCenter, GridPointFunc const & func ) 
	{
		for ( auto gp: (* m_pGridNeighbors)[ gpCenter.GetXvalue() ][ gpCenter.GetYvalue() ] )
		{
			func( gp );
		}
	};

	static int GetNrOfNeighbors( ) 
	{ 
		return m_iNrOfNeighbors; 
	}

	Neighborhood( )	: m_neighbors( )
	{
		m_neighbors.reserve( m_iNrOfNeighbors );
	}

    void AddToList( GridPoint const gp ) 
	{ 
		m_neighbors.push_back( gp ); 
		assert( static_cast<int>(m_neighbors.size()) <= m_iNrOfNeighbors );
	}

	void RemoveFromList( int const iIndex )
	{
		assert( static_cast<int>(m_neighbors.size()) >= iIndex);
		m_neighbors.erase( m_neighbors.begin() + iIndex );
	}

    void ClearList( ) 
	{ 
		m_neighbors.clear( ); 
	}

	size_t GetLength( ) const 
	{ 
		return m_neighbors.size( ); 
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
	using NEIGHBORS     = vector< GridPoint >;
	using NEIGHBOR_GRID = vector < vector < NEIGHBORS > >;

	static int             m_iNrOfNeighbors;
	static NEIGHBOR_GRID * m_pGridNeighbors;

    NEIGHBORS m_neighbors;
};
