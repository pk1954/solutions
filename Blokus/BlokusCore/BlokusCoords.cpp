// BlokusCoords.cpp
//
// BlokusCore

module BlokusCore:BlokusCoords;

import :Components;

MicroMeter const UM_CELL_SIZE      { 10000._MicroMeter };
MicroMeter const UM_CELL_SIZE_HALF { UM_CELL_SIZE * 0.5f };
MicroMeter const UM_BOARD_SIZE     { UM_CELL_SIZE * static_cast<float>(BOARD_SIZE) };

bool IsInShapeRange(CoordPos const &pos)
{
    return IsInRange(pos.GetX(), 0_COORD, MAX_COL) && IsInRange(pos.GetY(), 0_COORD, MAX_ROW);
}

MicroMeter Convert2fCoord(Coord const coord) 
{ 
	return UM_CELL_SIZE * static_cast<float>(coord.GetValue());
}

MicroMeterPnt Convert2fCoord(CoordPos const& coordPos)
{
	return MicroMeterPnt(Convert2fCoord(coordPos.GetX()), Convert2fCoord(coordPos.GetY()));
}

MicroMeterRect Convert2fCoord(CoordRect const& rect)
{
	return MicroMeterRect(Convert2fCoord(rect.GetStartPoint()), Convert2fCoord(rect.GetEndPoint()));
}

Coord Round2Coord(MicroMeter const &um)
{
	return Coord(Cast2SignedChar(((um + UM_CELL_SIZE_HALF) / UM_CELL_SIZE)));
}

CoordPos Round2CoordPos(MicroMeterPnt const& umPnt)
{
	return CoordPos(Round2Coord(umPnt.GetX()), Round2Coord(umPnt.GetY()));
}