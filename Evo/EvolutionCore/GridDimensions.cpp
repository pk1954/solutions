// GridDimensions.cpp
//
// EvolutionCore
//

module GridDimensions;

#include "grid_model.h"

import std;

GridPoint                       GridDimensions::m_gridSize       { GP_NULL }; 
int                             GridDimensions::m_iNrOfNeighbors { 0 }; 
GridDimensions::NEIGHBOR_GRID * GridDimensions::m_pGridNeighbors { nullptr };

static GridCoord const WEST     { -1_GRID_COORD };
static GridCoord const EAST     {  1_GRID_COORD };
static GridCoord const NORTH    { -1_GRID_COORD };
static GridCoord const SOUTH    {  1_GRID_COORD };
static GridCoord const CENTER_X {  0_GRID_COORD };
static GridCoord const CENTER_Y {  0_GRID_COORD };

static std::array< GridPoint, 8 > const table8 = 
{
	GridPoint(CENTER_X, NORTH   ), 
	GridPoint(WEST,     NORTH   ),
	GridPoint(WEST,     CENTER_Y),
	GridPoint(WEST,     SOUTH   ), 
	GridPoint(CENTER_X, SOUTH   ), 
	GridPoint(EAST,     SOUTH   ),
	GridPoint(EAST,     CENTER_Y),
	GridPoint(EAST,     NORTH   ) 
}; 

static std::array< GridPoint, 6 > const table6even = 
{
	GridPoint(CENTER_X, NORTH   ), 
	GridPoint(EAST,     CENTER_Y),
	GridPoint(EAST,     SOUTH   ), 
	GridPoint(CENTER_X, SOUTH   ), 
	GridPoint(WEST,     SOUTH   ),
	GridPoint(WEST,     CENTER_Y) 
}; 

static std::array< GridPoint, 6 > const table6odd = 
{
	GridPoint(CENTER_X, NORTH   ), 
	GridPoint(EAST,     NORTH   ), 
	GridPoint(EAST,     CENTER_Y),
	GridPoint(CENTER_X, SOUTH   ), 
	GridPoint(WEST,     CENTER_Y), 
	GridPoint(WEST,     NORTH   )
}; 

static std::array< GridPoint, 4 > const table4 = 
{
	GridPoint(CENTER_X, NORTH   ), 
	GridPoint(WEST,     CENTER_Y),
	GridPoint(CENTER_X, SOUTH   ), 
	GridPoint(EAST,     CENTER_Y)
}; 

void GridDimensions::DefineGridSize
(
	GridCoord const width, 
	GridCoord const height,
	int const iNrOfNeighbors 
)
{
	m_iNrOfNeighbors = iNrOfNeighbors;
	m_gridSize = GridPoint(width, height);
	Assert((iNrOfNeighbors == 4) || (iNrOfNeighbors == 6) || (iNrOfNeighbors == 8));

	m_pGridNeighbors = new NEIGHBOR_GRID;
	m_pGridNeighbors->resize(GridWidthVal());
	for (auto & col: * m_pGridNeighbors)
		col.resize(GridHeightVal());

	Apply2Grid  // initialization of grid variables which never change after initialization
	(
		[&](GridPoint const gp)
		{
			NEIGHBORS & neighbors = (* m_pGridNeighbors)[gp.GetXvalue()][gp.GetYvalue()];
			neighbors.resize(m_iNrOfNeighbors, GP_ZERO);
			for (int i = 0; i < m_iNrOfNeighbors; ++i)
			{
				GridPoint gpDelta = (m_iNrOfNeighbors == 6) 
					? (IsEvenColumn(gp))
					? table6even[i] 
					: table6odd[i]
					: (m_iNrOfNeighbors == 8) 
					? table8[i] 
					: table4[i];
				neighbors[i] = GridDimensions::Wrap2Grid(gp + gpDelta);
			}
		}
	);
}
