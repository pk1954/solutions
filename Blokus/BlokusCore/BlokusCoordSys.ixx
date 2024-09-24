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

	fPixelPoint SetOffset(fPixelPoint const& newOffset) 
	{ 
		fPixelPoint oldOffset = m_offset;
		m_offset = newOffset;
		return oldOffset; 
	}

	fPixelPoint Add2Offset(fPixelPoint const& addOffset) 
	{ 
		return SetOffset(m_offset + addOffset);
	}

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

    fPixelPoint GetCenter(CoordPos const& pos) const
	{
		fPixel      const fPixHalfSize { m_cellSize * 0.5f };
		fPixelPoint const fPos         { Transform2fPixelPos(pos) + fPixelPoint(fPixHalfSize) };
		return fPos;
	}

private:
	fPixelPoint m_offset;
	fPixel      m_cellSize;
};