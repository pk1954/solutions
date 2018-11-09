// win32_individualShape.h
//

#pragma once

#include <sstream> 
#include "GridPoint.h"
#include "win32_shape.h"
#include "win32_individualShape.h"

class PixelCoordinates;
class EvolutionCore;
class D3dBuffer;

class CoordShape : public Shape
{
public:
	CoordShape
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay
	) :
		Shape( pParent, textDisplay )
	{}

	void FillBuffer( GridPoint const gp )
	{
		m_textDisplay.Buffer() << gp;
	}

private:
};

class GridPointShape : public Shape
{
public:
	GridPointShape( TextDisplay & textDisplay ) :
		Shape( nullptr, textDisplay ),
		m_indivShape( this, textDisplay ),
		m_coordShape( this, textDisplay )
	{ }

	virtual void FillBuffer( GridPoint const gp ) { };  // all text in subshapes

	virtual void PrepareShape( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			PixelRectSize const rectSize = GetShapeSize();
			long          const lShapeWidth  = rectSize.GetWidth();
			long          const lShapeHeight = rectSize.GetHeight();
			long                lYpos        = 0;
			long                lHeight      = lShapeHeight / 10;

			m_coordShape.SetShapeRect
			( 
				PixelPoint   (           0, lYpos ),
				PixelRectSize( lShapeWidth, lHeight )
			);
			m_coordShape.PrepareShape( gp );
			lYpos += lHeight;

			m_indivShape.SetShapeRect
			( 
				rectSize.ToPixelPoint() *  3 / 16,    // 3 left + 3 right margin
				rectSize                * 10 / 16     // rest for iIdividualShape
			);
			m_indivShape.PrepareShape( gp );
		}
	}

	void Draw( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			m_coordShape.Draw( gp );
			m_indivShape.Draw( gp );
		}
	}

	Shape const * FindShape
	( 
		PixelPoint const pnt, 
		GridPoint  const gp
	) const
	{
		Shape const * pShapeRes;
    	short const   sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			pShapeRes = m_coordShape.FindShape( pnt, gp );
			if ( pShapeRes != nullptr )
				return pShapeRes;

			pShapeRes = m_indivShape.FindShape( pnt, gp );
			if ( pShapeRes != nullptr )
				return pShapeRes;
		}

		return PointInShape( pnt ) ? this : nullptr;
	}

	IndividualShape const & GetIndividualShape() const 
	{
		return m_indivShape;
	}

private:
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
