// win32_leftColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include <algorithm>  // min/max templates
#include "win32_leftColumn.h"

// Can be displayed, if at least InfoShape has space
// If possible, display also IdShape

PixelRectSize LeftColumn::MinimalSize( EvolutionCore const * const pCore )  
{       
	PixelRectSize const minId   = m_idShape  .MinimalSize( pCore );
	PixelRectSize const minInfo = m_infoShape.MinimalSize( pCore );

	return SetMinSize( minInfo );     
}                                     

void LeftColumn::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( SetShapeRect( ppOffset, ppSize ) )
	{
		PIXEL pixWidth    = getShapeWidth ();
		PIXEL pixHeight   = getShapeHeight();
		PIXEL pixIdHeight = m_idShape.GetMinHeight();

		PixelPoint posShape = GetShapePos( );

		if ( pixIdHeight + m_infoShape.GetMinHeight() <= pixHeight )
		{
			m_idShape  .PrepareShape( posShape, PixelRectSize( pixWidth,             pixIdHeight ) );
			posShape += PixelPoint( 0_PIXEL, pixIdHeight );
			m_infoShape.PrepareShape( posShape, PixelRectSize( pixWidth, pixHeight - pixIdHeight ) );
		}
		else
		{
			m_idShape  .SetShapeEmpty( );
			m_infoShape.PrepareShape( posShape, PixelRectSize( pixWidth, pixHeight ) );
		}
	}
}

void LeftColumn::Draw
( 
	EvolutionCore const * const pCore, 
	GridPoint             const gp, 
	PixelPoint            const pntGridpointOffset 
)
{
	if ( IsNotEmpty () )
	{
 		m_idShape  .Draw( pCore, gp, pntGridpointOffset );
		m_infoShape.Draw( pCore, gp, pntGridpointOffset );
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
