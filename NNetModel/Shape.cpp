// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "shape.h"

NNetModel               * Shape::m_pModel    { nullptr };
GraphicsInterface       * Shape::m_pGraphics { nullptr };
PixelCoordsFp     const * Shape::m_pCoord    { nullptr };

COLORREF Shape::GetFrameColor( ) const 
{ 
	return IsSuperHighlighted( ) 
		? EXT_COLOR_SUPER_HIGHLIGHT 
		: IsHighlighted( ) 
		? EXT_COLOR_HIGHLIGHT 
		: EXT_COLOR_NORMAL;
};

COLORREF Shape::GetInteriorColor( mV const voltageInput ) const
{
	mV       const voltage { min( voltageInput, NNetModel::GetPeakVoltage() ) };
	int      const colElem { CastToInt( voltage  * 255.0f / NNetModel::GetPeakVoltage()) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	return color;
}
