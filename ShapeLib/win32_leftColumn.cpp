// win32_leftColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include <algorithm>  // min/max templates
#include "win32_leftColumn.h"

// Can be displayed, if at least InfoShape has space
// If possible, display also IdShape

PixelRectSize LeftColumn::MinimalSize( )  
{       
	PixelRectSize const minId   = m_idShape  .MinimalSize( );
	PixelRectSize const minInfo = m_infoShape.MinimalSize( );

	return SetMinSize( minInfo );     
}                                     

void LeftColumn::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( SetShapeRect( ppOffset, ppSize ) )
	{
		long lWidth    = getShapeWidth ();
		long lHeight   = getShapeHeight();
		long lIdHeight = m_idShape.GetMinHeight();

		PixelPoint posShape = GetShapePos( );

		if ( lIdHeight + m_infoShape.GetMinHeight() <= lHeight )
		{
			m_idShape  .PrepareShape( posShape, PixelRectSize( lWidth,           lIdHeight ) );
			posShape.y += lIdHeight;
			m_infoShape.PrepareShape( posShape, PixelRectSize( lWidth, lHeight - lIdHeight ) );
		}
		else
		{
			m_idShape  .SetShapeEmpty( );
			m_infoShape.PrepareShape( posShape, PixelRectSize( lWidth, lHeight ) );
		}
	}
}

void LeftColumn::Draw( GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( IsNotEmpty () )
	{
 		m_idShape  .Draw( gp, ppGridpointOffset );
		m_infoShape.Draw( gp, ppGridpointOffset );
	}
}

Shape const * LeftColumn::FindShape
( 
	PixelPoint const pnt,
	GridPoint  const gp
) const
{
	Shape const * pShapeRes;
	pShapeRes = m_idShape.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	pShapeRes = m_infoShape.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	return Shape::FindShape( pnt, gp );
}
