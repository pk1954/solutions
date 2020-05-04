// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "tHighlightType.h"
#include "Direct2D.h"
#include "Knot.h"

void Knot::DrawExterior( D2D_driver const & graphics, PixelCoordsFp const & coord, tHighlightType const type ) const
{
	drawCircle
	( 
		graphics, 
		coord, 
		GetFrameColor( type ), 
		(type == tHighlightType::normal) ? GetExtension( ) : 30.0_MicroMeter 
	);
}

void Knot::DrawInterior( D2D_driver const & graphics, PixelCoordsFp const & coord ) const
{
	drawCircle( graphics, coord, GetInteriorColor( ), GetExtension( ) * PIPE_INTERIOR );
}
