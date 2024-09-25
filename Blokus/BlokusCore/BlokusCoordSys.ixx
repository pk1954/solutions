// BlokusCoordSys.ixx
//
// BlokusCore

export module BlokusCore:BlokusCoordSys;

import std;
import Util;
import Types;
import SaveCast;
import :BlokusCoords;

export class BlokusCoordSys
{
public:
	void Reset(fPixelPoint const& offset, fPixel const cellSize)
	{
		m_offset   = offset;
		m_cellSize = cellSize;
	}

	fPixel             CellSize() const { return m_cellSize; }
	fPixelPoint const& Offset  () const { return m_offset; }

	void        SetCellSize(fPixel const newSize) { m_cellSize = newSize; };
	fPixelPoint SetOffset(fPixelPoint const& newOffset) 
	{ 
		fPixelPoint oldOffset = m_offset;
		m_offset = newOffset;
		return oldOffset; 
	}

	fPixelPoint Add2Offset(fCoordPos const &addOffset) 
	{ 
		fPixelPoint fPixPnt { Transform2fPixelSize(addOffset) };
		return SetOffset(m_offset + fPixPnt);
	}

	fPixelPoint Transform2fPixelSize(fCoordPos const& coordPos) const
	{
		return fPixelPoint
		(
			m_cellSize * Cast2Float(coordPos.GetXvalue()),
			m_cellSize * Cast2Float(coordPos.GetYvalue())
		);
	}

	fPixelRect Transform2fPixelRect(fCoordRect const& rect) const
	{
		return fPixelRect
		(
			Transform2fPixelPos(rect.GetStartPoint()), 
			Transform2fPixelPos(rect.GetEndPoint  ()) 
		);
	}

	fPixelPoint Transform2fPixelPos(fCoordPos const& coordPos) const
	{
		return Transform2fPixelSize(coordPos) + m_offset;
	}

    fPixelPoint GetCenter(fCoordPos const& pos) const
	{
		fPixel      const fPixHalfSize { m_cellSize * 0.5f };
		fPixelPoint const fPos         { Transform2fPixelPos(pos) + fPixelPoint(fPixHalfSize) };
		return fPos;
	}

private:
	fPixelPoint m_offset;
	fPixel      m_cellSize;
};