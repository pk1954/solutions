// win32_individualShape.h
//

#pragma once

#include "GridPoint.h"
#include "win32_shape.h"
#include "win32_coordShape.h"
#include "win32_individualShape.h"

class PixelCoordinates;
class EvolutionCore;
class D3dBuffer;

class GridPointShape : public Shape
{
public:
	GridPointShape( TextDisplay & textDisplay ) :
		Shape( nullptr, textDisplay ),
		m_indivShape( this, textDisplay ),
		m_coordShape( this, textDisplay )
	{ }

	virtual void FillBuffer  ( GridPoint const gp ) { };  // all text in subshapes
	virtual void PrepareShape( GridPoint const );
	virtual void Draw        ( GridPoint const );

	virtual Shape const * FindShape( PixelPoint const, GridPoint const ) const;

	IndividualShape const & GetIndividualShape() const 
	{
		return m_indivShape;
	}

private:
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
