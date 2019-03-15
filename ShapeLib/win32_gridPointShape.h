// win32_individualShape.h
//

#pragma once

#include "win32_shape.h"
#include "win32_coordShape.h"
#include "win32_individualShape.h"
#include "gridPoint.h"

class PixelCoordinates;
class EvolutionCore;
class D3D_driver;

class GridPointShape
{
public:
	GridPointShape( TextDisplay & t ) :
		m_shape( t ),
		m_indivShape( t ),
		m_coordShape( t )
	{ }

	void          Draw( GridPoint const, PixelPoint const );
	Shape const * FindShape( PixelPoint const, GridPoint const ) const;
	void          RefreshLayout( );
	PIXEL         GetIndShapeSize( );

	IndividualShape const & GetIndividualShape() const 
	{
		return m_indivShape;
	}

private:
	Shape           m_shape;
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
