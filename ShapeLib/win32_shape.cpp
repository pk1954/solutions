// win32_shape.cpp
//

#pragma once

#include "stdafx.h"
#include "win32_shape.h"

void Shape::Draw( GridPoint const gp, PixelPoint const ppGridPointOffset )
{
	if ( IsNotEmpty() )
	{
		m_textDisplay.Clear();
		FillBuffer( gp );
		m_textDisplay.DisplayText( m_rect + ppGridPointOffset );
	}
}
