// win32_individualShape.cpp
//

#include "stdafx.h"
#include "win32_individualShape.h"

PixelRectSize IndividualShape::MinimalSize( )  
{       
	PixelRectSize minLeft  = m_leftColumn .MinimalSize( );
	PixelRectSize minRight = m_rightColumn.MinimalSize( );

	return setMinSize( minLeft );
}                                     

void IndividualShape::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( setShapeRect( ppOffset, ppSize ) )
	{
		long lWidth  = getShapeWidth();
		long lHeight = getShapeHeight();

		PixelPoint    pixPosSubShape  = getShapePos( );
		PixelRectSize pixSizeSubShape = PixelRectSize( lWidth / 2, lHeight );

		if ( m_leftColumn.GetMinWidth() + m_rightColumn.GetMinWidth() <= lWidth )
		{
			m_leftColumn .PrepareShape( pixPosSubShape, pixSizeSubShape );
			pixPosSubShape.x += lWidth / 2;
			m_rightColumn.PrepareShape( pixPosSubShape, pixSizeSubShape );
		}
		else
		{
			m_leftColumn .PrepareShape( pixPosSubShape, PixelRectSize( lWidth, lHeight ) );
			m_rightColumn.SetShapeEmpty( );
		}
	}
}

void IndividualShape::Draw( GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( isNotEmpty () )
	{
		m_leftColumn. Draw( gp, ppGridpointOffset );
		m_rightColumn.Draw( gp, ppGridpointOffset );
	}
}

Shape const * IndividualShape::FindShape
( 
	PixelPoint const pnt, 
	GridPoint  const gp
) const
{
	Shape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	if ( m_textDisplay.GetStrategyId( gp ) == tStrategyId::tit4tat )
	{
		pShapeRes = m_rightColumn.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;
	}

	return Shape::FindShape( pnt, gp );
}
