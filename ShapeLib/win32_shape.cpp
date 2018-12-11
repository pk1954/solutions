// win32_shape.cpp
//

#pragma once

#include "stdafx.h"
#include "win32_shape.h"

PixelRect Shape::GetAbsoluteCoordinates( ) const
{
	PixelRect pRes( m_rect );
	for ( Shape * pParent = m_pParent; pParent; pParent = pParent->m_pParent )
		pRes += pParent->m_rect.GetStartPoint();
	return pRes;
}

void Shape::Draw( GridPoint const gp )
{
	m_textDisplay.Clear();
	FillBuffer( gp );
	PixelRect rectAbsolute = GetAbsoluteCoordinates( );
	m_textDisplay.DrawText( rectAbsolute );
}
