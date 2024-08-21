// GridNeighbor.ixx
//
// EvolutionCore

module EvolutionCore:;

import Debug;

export module EvolutionCore:GridNeighbor;

import std;
import :GridPoint;
import :GridModel;

using std::vector;

export using NEIGHBORS = vector<GridPoint>;

export class Neighborhood
{
public:
    static void InitClass(int const);

	static int GetNrOfNeighbors() 
	{ 
		return m_iNrOfNeighbors; 
	}

	Neighborhood()	: m_neighbors()
	{
		m_neighbors.reserve(m_iNrOfNeighbors);
	}

	void AddToList(GridPoint const & gp)
	{
		Assert(static_cast<int>(m_neighbors.size()) < m_iNrOfNeighbors);
		m_neighbors.push_back(gp);
	}

	void RemoveFromList(function<bool(GridPoint const)> const & func)
	{
		m_neighbors.erase(std::remove_if(m_neighbors.begin(), m_neighbors.end(), func), m_neighbors.end()); 
	}

	size_t GetLength() const 
	{ 
		return m_neighbors.size(); 
	}

	void Clear() 
	{ 
		return m_neighbors.clear(); 
	}

	GridPoint const GetElement(unsigned int const uiIndex ) const 
	{ 
		return m_neighbors[uiIndex]; 
	}

	GridPoint const GetRandomElement(unsigned int const uiRandom) const 
	{
		Assert(m_neighbors.size() > 0); 
		return m_neighbors[uiRandom % m_neighbors.size()];
	}

	bool const Includes(GridPoint const & gp) const 
	{
		return std::find(m_neighbors.begin(), m_neighbors.end(), gp) != m_neighbors.end();
	}

	void Apply2All(function<void(GridPoint const)> const & func)
	{
		for (auto gp : m_neighbors)
		{
			func(gp);
		}
	}

private:

	static int m_iNrOfNeighbors;

    NEIGHBORS m_neighbors;
};
