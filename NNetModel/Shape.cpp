// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "shape.h"

GraphicsInterface * Shape::m_pGraphics { nullptr };

COLORREF Shape::GetFrameColor( ) const 
{ 
	return IsSuperHighlighted( ) 
		? EXT_COLOR_SUPER_HIGHLIGHT 
		: IsHighlighted( ) 
		? EXT_COLOR_HIGHLIGHT 
		: EXT_COLOR_NORMAL;
};

COLORREF Shape::GetInteriorColor
(
	NNetModel const & model,
	mV        const voltageInput 
) const
{
	mV       const voltage { min( voltageInput, model.GetPeakVoltage() ) };
	int      const colElem { CastToInt( voltage  * 255.0f / model.GetPeakVoltage()) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	return color;
}
