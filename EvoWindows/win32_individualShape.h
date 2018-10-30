// win32_individualShape.h
//

#pragma once

#include <sstream> 
#include "GridPoint.h"
#include "win32_rectShape.h"
#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

class PixelCoordinates;
class EvolutionCore;
class D3dBuffer;
class LeftColumn;
class RightColumn;

class IndividualShape : public RectShape
{
public:
	IndividualShape
	( 
		D3dBuffer        * const pD3dBuffer, 
		wostringstream         & wBuffer, 
		EvolutionCore    * const pCore,
		PixelCoordinates * const pPixelCoordinates
	) :
		m_leftColumn ( pD3dBuffer, wBuffer, pCore ),
		m_rightColumn( pD3dBuffer, wBuffer, pCore ),
		m_pPixelCoordinates( pPixelCoordinates ),
		m_pCore( pCore )
	{ }

	virtual void PrepareShape( GridPoint const gp  )
	{
		m_lSizeInd = (5 * m_pPixelCoordinates->GetFieldSize()) / 8;                    // use only 5/8 of field size
		m_offset   = m_pPixelCoordinates->Grid2PixelPosCenter( gp ) - m_lSizeInd / 2;
	}

	virtual void Draw( GridPoint const ) = 0;

protected:

	LeftColumn  m_leftColumn;
	RightColumn m_rightColumn;
	PixelPoint  m_offset;
	long        m_lSizeInd;

	EvolutionCore const * const m_pCore;

private:

    PixelCoordinates * const m_pPixelCoordinates;
};

class IndividualShape_Level_1 : public IndividualShape
{
public:
	using IndividualShape::IndividualShape;

	void Draw( GridPoint const gp )
	{
		PrepareShape( gp );
		m_leftColumn.SetSize( PixelRectSize( m_lSizeInd, m_lSizeInd ) );
		m_leftColumn.SetOffset( PixelPoint( 0, 0 ) );
		m_leftColumn.Draw( gp, m_offset );
	}
};

class IndividualShape_Level_2 : public IndividualShape
{
public:
	using IndividualShape::IndividualShape;

	void Draw( GridPoint const gp )
	{
		PrepareShape( gp );
		m_leftColumn .SetSize( PixelRectSize( m_lSizeInd / 2, m_lSizeInd ) );
		m_rightColumn.SetSize( PixelRectSize( m_lSizeInd / 2, m_lSizeInd ) );
		m_leftColumn .SetOffset( PixelPoint( 0, 0 ) );
		m_rightColumn.SetOffset( PixelPoint( m_lSizeInd / 2, 0 ) );

		m_leftColumn.Draw( gp, m_offset );

		if ( m_pCore->GetStrategyId( gp ) == tStrategyId::tit4tat )
		{
			m_rightColumn.Draw( gp, m_offset );
		}
	}
};
