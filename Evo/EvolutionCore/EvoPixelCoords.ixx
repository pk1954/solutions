// EvoPixelCoords.ixx
//
// EvolutionCore

module;

export module EvoPixelCoords;

import Types;
import SmoothMoveFp;
import GridPoint;
import GridRect;

export class EvoPixelCoords
{
public:

	EvoPixelCoords();

	void Start(PIXEL const, bool const);

	//////// transformations ////////

	PixelPoint Grid2PixelSize(GridPoint  const) const;

	GridPoint  Pixel2GridPos  (PixelPoint const) const;
	PixelPoint Grid2PixelPos  (GridPoint  const) const; 

    PixelRect  Grid2PixelRect (GridRect  const &) const;
    GridRect   Pixel2GridRect (PixelRect const &) const;

	PixelPoint Grid2PixelPosCenter(GridPoint const) const; 

	//////// queries ////////
	
	PIXEL      GetFieldSize()   const { return m_pixFieldSize; };
	PixelPoint GetPixelOffset() const { return m_pixOffset; }

	PIXEL      ComputeNewFieldSize(bool const) const;  // does not modify field size
    PIXEL	   CalcMaximumFieldSize(GridRectSize const &, PixelRectSize const &) const;

	//////// manipulation functions ////////

    bool  SetGridFieldSize(PIXEL const);                                   
	void  CenterGrid(GridPoint const, PixelRectSize const);

	bool  CenterPoi(PixelPoint const, GridPoint const);
    void  MoveGrid(PixelPoint const);

private:

	bool       isValidFieldSize(PIXEL const) const; 
    PixelPoint calcCenterOffset(GridPoint const, PixelPoint const);

    PixelPoint   m_pixOffset;
    PIXEL        m_pixFieldSize;
    SmoothMoveFp m_smoothMove;
    bool         m_bMoving;
	bool         m_bHexagon;
};

PIXEL const MINIMUM_FIELD_SIZE =    1_PIXEL;
PIXEL const DEFAULT_FIELD_SIZE =    8_PIXEL;
PIXEL const MAXIMUM_FIELD_SIZE = 1024_PIXEL;

PixelPoint EvoPixel2PixelSize(PixelPoint const,   EvoPixelCoords const *, EvoPixelCoords const *);
PixelPoint EvoPixel2PixelPos (PixelPoint const,   EvoPixelCoords const *, EvoPixelCoords const *);
PixelRect  EvoPixel2PixelRect(PixelRect  const &, EvoPixelCoords const *, EvoPixelCoords const *);
