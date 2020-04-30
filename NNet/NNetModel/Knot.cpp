// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "PixelCoordsFp.h"
#include "tHighlightType.h"
#include "Direct2D.h"
#include "Knot.h"

void Knot::DrawExterior( D2D_driver const * pGraphics, PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawCircle
	( 
		pGraphics, 
		coord, 
		GetFrameColor( type ), 
		(type == tHighlightType::normal) ? GetExtension( ) : 30.0_MicroMeter 
	);
}

void Knot::DrawInterior( D2D_driver const * pGraphics, PixelCoordsFp & coord )
{
	drawCircle( pGraphics, coord, GetInteriorColor( ), GetExtension( ) * PIPE_INTERIOR );
}
