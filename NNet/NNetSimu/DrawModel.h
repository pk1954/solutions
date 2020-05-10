// DrawModel.h
//
// NNetSimu

#pragma once

#include "DrawContext.h"

class NNetModel;

class DrawModel
{
public:

DrawModel( NNetModel const * const pModel )
	: m_pModel( pModel )
{}

ShapeId const FindShapeAt         ( PixelPoint const &, DrawContext const &, ShapeCrit const & ) const;
void          DrawInteriorInRect  ( PixelRect  const &, DrawContext const &, ShapeCrit const & ) const;
void          DrawExteriorInRect  ( PixelRect  const &, DrawContext const &                    ) const;
void          DrawNeuronTextInRect( PixelRect  const &, DrawContext const &                    ) const;


private:
	NNetModel const * m_pModel;
};