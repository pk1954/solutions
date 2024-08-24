// GridDimensions.ixx
//
// EvoCoreLib
//
// knows static size of the GridModel and number of neighbors
// contains functions directly dependent on GridModel size

export module EvoCoreLib:GridDimensions;

import std;
import :GridRect;
import :GridCoord;
import :GridNeighborhood;

using std::min;
using std::max;
using std::vector;

export class GridDimensions
{
public:
	static void DefineGridSize
	(
		GridCoord const, 
		GridCoord const,
		int        const
	);

	static int       const GetNrOfNeigbors() { return m_iNrOfNeighbors; }
	static GridCoord const GridWidth()       { return m_gridSize.GetX(); }
	static GridCoord const GridHeight()      { return m_gridSize.GetY(); }
	static GridCoord const GridMinX()        { return 0_GRID_COORD; }
	static GridCoord const GridMinY()        { return 0_GRID_COORD; }
	static GridCoord const GridMaxX()        { return GridWidth()  - 1_GRID_COORD; }
	static GridCoord const GridMaxY()        { return GridHeight() - 1_GRID_COORD; }
	static GridPoint const GridRectSize()    { return m_gridSize; }
	static GridPoint const GridOrigin()      { return GP_ZERO; }
	static GridPoint const GridMaximum()     { return GridPoint(GridMaxX(), GridMaxY()); }
	static GridRect  const GridRectFull()    { return GridRect(GridOrigin(), GridMaximum()); }

	static int const GridWidthVal()  { return GridWidth() .GetValue(); }; // abbreviations for e.g. array dims
	static int const GridHeightVal() { return GridHeight().GetValue(); };
	static int const GetGridArea()   { return GridWidthVal() * GridHeightVal(); };

	static inline GridPoint const Wrap2Grid(GridPoint const gp) 
	{ 
		return (gp + m_gridSize) % m_gridSize; 
	}

	static NEIGHBORS const & GetNeighbors(GridPoint const gp)
	{
		return (* m_pGridNeighbors)[gp.GetXvalue()][gp.GetYvalue()];
	}

	static bool const IsHexMode()
	{
		return m_iNrOfNeighbors == 6;
	}

private:
	using NEIGHBOR_GRID = vector<vector<NEIGHBORS>>;

	static GridPoint       m_gridSize; 
	static int             m_iNrOfNeighbors;
	static NEIGHBOR_GRID * m_pGridNeighbors;
};

export inline bool const Neighbors(GridPoint const a, GridPoint const b)
{ 
	GridPoint const gpDiff { a - b };
	GridCoord const dx     { GridCoord(std::abs(gpDiff.GetXvalue())) }; 
	GridCoord const dy     { GridCoord(std::abs(gpDiff.GetYvalue())) };
    return (
		      ((dx <= 1_GRID_COORD) || (dx == GridDimensions::GridMaxX())) && 
		      ((dy <= 1_GRID_COORD) || (dy == GridDimensions::GridMaxY()))
		  );
}

export inline GridPoint const ClipToGrid(GridPoint gp) 
{ 
	gp = GridPoint
	(
		max(gp.GetX(), GridDimensions::GridMinX()),
		max(gp.GetY(), GridDimensions::GridMinY())
	);

	gp = GridPoint
	(
		min(gp.GetX(), GridDimensions::GridMaxX()),
		min(gp.GetY(), GridDimensions::GridMaxY())
	);

	return gp;	
}

export inline bool const IsInGrid(GridPoint const & gp) 
{ 
	return ClipToGrid(gp) == gp;
};

export inline void Apply2Rect(GridPointFunc const & func, GridRect const & rect)
{
	::Apply2Rect
	(
		func, 
		ClipToGrid(rect.GetStartPoint()),
		ClipToGrid(rect.GetEndPoint  ())
	);
}

export inline void Apply2Grid(GridPointFunc const & func, bool const fWithBorders = false)
{
	Apply2Rect(func, GridDimensions::GridOrigin(), GridDimensions::GridMaximum(), fWithBorders);
}
