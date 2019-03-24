// win32_individualShape.cpp
//

#include "stdafx.h"
#include "win32_individualShape.h"

PixelRectSize IndividualShape::MinimalSize( )  
{       
	PixelRectSize minLeft  { m_leftColumn .MinimalSize( ) };
	PixelRectSize minRight { m_rightColumn.MinimalSize( ) };

	return SetMinSize( minLeft );
}                                     

void IndividualShape::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( SetShapeRect( ppOffset, ppSize ) )
	{
		PIXEL_X pixWidth  { getShapeWidth()  };
		PIXEL_Y pixHeight { getShapeHeight() };

		PixelPoint    pixPosSubShape  { GetShapePos( ) };
		PixelRectSize pixSizeSubShape { PixelRectSize( pixWidth / 2, pixHeight ) };

		if ( m_leftColumn.GetMinWidth() + m_rightColumn.GetMinWidth() <= pixWidth )
		{
			m_leftColumn .PrepareShape( pixPosSubShape, pixSizeSubShape );
			pixPosSubShape += PixelPoint( pixWidth / 2, 0_PIXEL_Y );
			m_rightColumn.PrepareShape( pixPosSubShape, pixSizeSubShape );
		}
		else
		{
			m_leftColumn .PrepareShape( pixPosSubShape, PixelRectSize( pixWidth, pixHeight ) );
			m_rightColumn.SetShapeEmpty( );
		}
	}
}

void IndividualShape::Draw( EvolutionCore const * const pCore, GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( IsNotEmpty () )
	{
		m_leftColumn. Draw(        gp, ppGridpointOffset );
		m_rightColumn.Draw( pCore, gp, ppGridpointOffset );
	}
}

Shape const * IndividualShape::FindShape
( 
	EvolutionCore const * const pCore, 
	PixelPoint    const         pnt, 
	GridPoint     const         gp
) const
{
	Shape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	if ( pCore->GetStrategyId( gp ) == Strategy::Id::tit4tat )
	{
		pShapeRes = m_rightColumn.FindShape( pCore, pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;
	}

	return Shape::FindShape( pnt, gp );
}
