// win32_shape.cpp
//

#pragma once

#include "stdafx.h"
#include "win32_shape.h"

void Shape::Draw
(
	EvolutionCore const * const pCore, 
	GridPoint             const gp, 
	PixelPoint            const ppGridPointOffset 
)
{
	if ( IsNotEmpty() )
	{
		m_pTextDisplay->Clear();
		FillBuffer( pCore, gp );
		m_pTextDisplay->DisplayText( m_rect + ppGridPointOffset );
	}
}
