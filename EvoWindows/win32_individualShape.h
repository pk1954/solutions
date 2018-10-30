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

	void SetClientWinHeight( long const lHeight )
	{ 
		m_lClientWinHeight = lHeight; 
	}
	
	virtual void Resize( short const sFieldSize )
	{
		m_lSizeInd = (5 * sFieldSize) / 8;
		m_leftColumn.SetOffset( PixelPoint( 0, 0 ) );
	}

	virtual void Draw( GridPoint const ) = 0;

protected:
	PixelPoint CalculateOffset( GridPoint const gp )
	{
		PixelPoint ptCenter   = m_pPixelCoordinates->Grid2PixelPosCenter( gp );
				   ptCenter.y = m_lClientWinHeight - ptCenter.y;     // because of DirectX coord system

		return ptCenter - m_lSizeInd / 2;
	}

	LeftColumn  m_leftColumn;
	RightColumn m_rightColumn;
	long        m_lSizeInd;

	EvolutionCore const * const m_pCore;

private:

	long                     m_lClientWinHeight;
    PixelCoordinates * const m_pPixelCoordinates;
};

class IndividualShape_Level_1 : public IndividualShape
{
public:
	using IndividualShape::IndividualShape;

	void Resize( short const sFieldSize )
	{
		IndividualShape::Resize( sFieldSize );
		m_leftColumn.SetSize( PixelRectSize( m_lSizeInd, m_lSizeInd ) );
	}

	void Draw( GridPoint const gp )
	{
		PixelPoint offset = CalculateOffset( gp );
		m_leftColumn.Draw( gp, offset );
	}
};

class IndividualShape_Level_2 : public IndividualShape
{
public:
	using IndividualShape::IndividualShape;

	void Resize( short const sFieldSize )
	{
		IndividualShape::Resize( sFieldSize );
		m_leftColumn .SetSize( PixelRectSize( m_lSizeInd / 2, m_lSizeInd ) );
		m_rightColumn.SetSize( PixelRectSize( m_lSizeInd / 2, m_lSizeInd ) );
		m_rightColumn.SetOffset( PixelPoint( m_lSizeInd / 2, 0 ) );
	}

	void Draw( GridPoint const gp )
	{
		PixelPoint offset = CalculateOffset( gp );

		m_leftColumn.Draw( gp, offset );

		if ( m_pCore->GetStrategyId( gp ) == tStrategyId::tit4tat )
		{
			m_rightColumn.Draw( gp, offset );
		}
	}
};
