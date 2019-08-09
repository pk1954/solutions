// win32_individualShape.h
//

#pragma once

#include "win32_shape.h"
#include "win32_coordShape.h"
#include "win32_individualShape.h"
#include "gridPoint.h"

class TextDisplay;
class PixelCoordinates;
class EvolutionCore;
class D3D_driver;

class GridPointShape
{
public:
	GridPointShape( )
		: m_pTextDisplay( nullptr )
	{ }

	void Start( TextDisplay * pTextDisplay )
	{
		m_pTextDisplay = pTextDisplay;
		m_shape.SetTextDisplay( pTextDisplay );
		m_coordShape.SetTextDisplay( pTextDisplay );
		m_indivShape.SetTextDisplay( pTextDisplay );
	}

	void          Draw         ( EvolutionCore const * const, GridPoint const );
	Shape const * FindShape    ( EvolutionCore const * const, GridPoint const, PixelPoint const ) const;
	void          RefreshLayout( EvolutionCore const * const );
	PIXEL         GetIndShapeSize( );

	IndividualShape const & GetIndividualShape() const 
	{
		return m_indivShape;
	}

private:
	TextDisplay   * m_pTextDisplay;
	Shape           m_shape;
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
