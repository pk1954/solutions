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
	GridPointShape( TextDisplay & t ) :
		Shape( t ),
		m_indivShape( t ),
		m_coordShape( t )
	{ }

	virtual PixelRectSize MinimalSize( );
	virtual void          FillBuffer( GridPoint const gp ) { };  // all text in subshapes
	virtual void          Draw      ( GridPoint const, PixelPoint const );

	virtual Shape const * FindShape( PixelPoint const, GridPoint const ) const;

	void RefreshLayout( );
	long GetIndShapeSize( );

	IndividualShape const & GetIndividualShape() const 
	{
		return m_indivShape;
	}

private:
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
