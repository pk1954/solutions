// BlokusCoords.cpp
//
// BlokusCore

module BlokusCore:BlokusCoords;

import :Components;

MicroMeter const UM_CELL_SIZE  { 10000._MicroMeter };
MicroMeter const UM_BOARD_SIZE { UM_CELL_SIZE * static_cast<float>(BOARD_SIZE) };

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