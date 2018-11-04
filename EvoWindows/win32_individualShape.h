// win32_individualShape.h
//

#pragma once

#include <sstream> 
#include "GridPoint.h"
#include "win32_gridPointShape.h"
#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

class PixelCoordinates;
class EvolutionCore;
class D3dBuffer;
class LeftColumn;
class RightColumn;

class IndividualShape : public GridPointShape
{
public:
	using GridPointShape::GridPointShape;

	virtual void FillBuffer( GridPoint const gp ) { };  // all text in subshapes

	virtual void Draw( GridPoint const ) = 0;

	virtual GridPointShape const * FindRelevantShape( PixelPoint const &, GridPoint const ) const = 0;
};

class Zoom_level_0 : public IndividualShape
{
public:
	Zoom_level_0( TextDisplay & textDisplay ) :
		IndividualShape( nullptr, textDisplay )
	{ }

	GridPointShape const * FindRelevantShape
	( 
		PixelPoint const & pnt,            
		GridPoint  const   gp
	) const
	{
		return nullptr;
	}

	PixelRect const GetRect( ) const
	{
		return PixelRect();   // return empty rect
	}

	void Draw( GridPoint const gp )
	{
		// individuals are too small to display any text
	}
};

class Zoom_level_1 : public IndividualShape
{
public:
	Zoom_level_1( TextDisplay & textDisplay ) :
		IndividualShape( nullptr, textDisplay ),
		m_leftColumn   ( this, textDisplay ),
		m_gpCoordShape ( this, textDisplay )
	{ }

	virtual void PrepareShape( GridPoint const gp )
	{
		PixelRectSize const rectSize = GetSize();
		long          const lWidth   = rectSize.GetWidth();
		long          const lHeight  = rectSize.GetHeight();

		m_gpCoordShape.SetSize( PixelRectSize( lWidth, lHeight / 10 ) );
		m_gpCoordShape.SetShapeOffset( PixelPoint( 0, 11 * lHeight / 10 ) ); 
		m_gpCoordShape.PrepareShape( gp );

		m_leftColumn.SetSize( PixelRectSize( lWidth, lHeight ) );
		m_leftColumn.SetShapeOffset( PixelPoint( 0, 0 ) );
		m_leftColumn.PrepareShape( gp );
	}

	GridPointShape const * FindRelevantShape
	( 
		PixelPoint const & pnt,
		GridPoint  const   gp
	) const
	{
		GridPointShape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;

		return PointInShape( pnt ) ? this : nullptr;
	}

	void Draw( GridPoint const gp )
	{
		m_leftColumn.Draw( gp );
	}

private:
	GridPointCoordShape m_gpCoordShape;
	LeftColumn          m_leftColumn;
};

class Zoom_level_2 : public IndividualShape
{
public:
	Zoom_level_2( TextDisplay & textDisplay ) :
		IndividualShape( nullptr, textDisplay ),
		m_leftColumn ( this, textDisplay ),
		m_rightColumn( this, textDisplay ),
		m_gpCoordShape ( this, textDisplay )
	{ }

	virtual void PrepareShape( GridPoint const gp )
	{
		PixelRectSize const rectSize = GetSize();
		long          const lWidth   = rectSize.GetWidth();
		long          const lHeight  = rectSize.GetHeight();

		m_gpCoordShape.SetSize( PixelRectSize( lWidth, lHeight / 10 ) );
		m_gpCoordShape.SetShapeOffset( PixelPoint( 0, 11 * lHeight / 10 ) );
		m_gpCoordShape.PrepareShape( gp );

		m_leftColumn.SetSize( PixelRectSize( lWidth / 2, lHeight ) );
		m_leftColumn.SetShapeOffset( PixelPoint( 0, 0 ) );
		m_leftColumn.PrepareShape( gp );

		m_rightColumn.SetSize( PixelRectSize( lWidth / 2, lHeight ) );
		m_rightColumn.SetShapeOffset( PixelPoint( lWidth / 2, 0 ) );
		m_rightColumn.PrepareShape( gp );
	}

	GridPointShape const * FindRelevantShape
	( 
		PixelPoint const & pnt, 
		GridPoint  const   gp
	) const
	{
		GridPointShape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;

		if ( m_textDisplay.Core().GetStrategyId( gp ) == tStrategyId::tit4tat )
		{
			pShapeRes = m_rightColumn.FindShape( pnt, gp );
			if ( pShapeRes != nullptr )
				return pShapeRes;
		}

		return PointInShape( pnt ) ? this : nullptr;
	}

	void Draw( GridPoint const gp )
	{
		m_gpCoordShape.Draw( gp );
		m_leftColumn.Draw( gp );

		if ( m_textDisplay.Core().GetStrategyId( gp ) == tStrategyId::tit4tat )
		{
			m_rightColumn.Draw( gp );
		}
	}

private:
	GridPointCoordShape m_gpCoordShape;
	LeftColumn          m_leftColumn;
	RightColumn         m_rightColumn;
};
