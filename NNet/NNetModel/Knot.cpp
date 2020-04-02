// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "tHighlightType.h"
#include "Direct2D.h"
#include "Knot.h"

void Knot::DrawExterior( PixelCoordsFp & coord, tHighlightType const type ) const
{
	drawCircle
	( 
		coord, 
		GetFrameColor( type ), 
		(type == tHighlightType::normal) ? GetExtension( ) : 30.0_MicroMeter 
	);
}

void Knot::DrawInterior( PixelCoordsFp & coord )
{
	drawCircle( coord, GetInteriorColor( ), GetExtension( ) * PIPELINE_INTERIOR );
}
