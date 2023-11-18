// gridRect.cpp :
//
// EvolutionCore

module GridRect;

void Apply2Rect
(
	GridPointFunc const & func,
	GridPoint             gpStart,
	GridPoint             gpEnd,
	bool          const   fWithBorders
)
{
	if (fWithBorders)
	{
		gpStart -= GridPoint(1_GRID_COORD);
		gpEnd   += GridPoint(1_GRID_COORD);
	}

    for (GridCoord y = gpStart.GetY(); y <= gpEnd.GetY(); ++y)
    for (GridCoord x = gpStart.GetX(); x <= gpEnd.GetX(); ++x)
	{
		func(GridPoint(x, y));
	}
}
