// BlokusCoordSys.ixx
//
// BlokusCore

export module BlokusCore:BlokusCoordSys;

import std;
import Util;
import Types;
import SaveCast;
import :CoordPos;

export class BlokusCoordSys
{
public:
	void Reset
	(
		fPixelPoint offset,
		fPixel      cellSize
	)
	{
		m_offset   = offset;
		m_cellSize = cellSize;
	}

	fPixel             CellSize() const { return m_cellSize; }
	fPixelPoint const& Offset  () const { return m_offset; }

	fPixelPoint Transform2fPixelSize(CoordPos const& coordPos) const
	{
		return fPixelPoint
		(
			m_cellSize * Cast2Float(coordPos.GetXvalue()),
			m_cellSize * Cast2Float(coordPos.GetYvalue())
		);
	}

	fPixelPoint Transform2fPixelPos(CoordPos const& coordPos) const
	{
		return Transform2fPixelSize(coordPos) + m_offset;
	}

private:
	fPixelPoint m_offset;
	fPixel      m_cellSize;
};