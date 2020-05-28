// DrawModel.h
//
// NNetSimu

#pragma once

#include "Shape.h"
#include "DrawContext.h"

class NNetModel;
class PixelTypes;

class DrawModel
{
public:

void Initialize( NNetModel const * const pModel )
{
	m_pModel = pModel;
}

ShapeId const FindShapeAt         ( PixelPoint const &, DrawContext const &, ShapeCrit const & ) const;
void          DrawInteriorInRect  ( PixelRect  const &, DrawContext const &, ShapeCrit const & ) const;
void          DrawExteriorInRect  ( PixelRect  const &, DrawContext const &                    ) const;
void          DrawNeuronTextInRect( PixelRect  const &, DrawContext const &                    ) const;

private:
	NNetModel const * m_pModel { nullptr };
};