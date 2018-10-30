// win32_shape.h : 
//

#pragma once

#include "vector"
#include "pixelPoint.h"

using namespace std;

class Shape
{
public:
	Shape() :
		m_shapeOffset( PixelPoint( 0, 0 ) )
	{}

	// PointInShape: 
	// all parameters are relative to client rect

	virtual bool PointInShape( PixelPoint const &, PixelPoint const & ) const = 0;

	////////////////////

	Shape const * FindSubshape
	( 
		PixelPoint const & inheritedOffset, // offset of superior shape relative to client rect
		PixelPoint const & pnt              // point to be searched for, coordinates relative to client rect
	) const
	{
		PixelPoint offset( m_shapeOffset + inheritedOffset );
		for ( auto &shape : m_subShapes )
			if ( shape->PointInShape( offset, pnt ) )
				return shape;
		return nullptr;
	}

	PixelPoint GetOffset( ) const
	{
		return m_shapeOffset;
	}

	void SetOffset( PixelPoint const offset )
	{
		m_shapeOffset = offset;
	}

	void AddSubShape
	( 
		Shape          * pShape, 
		PixelPoint const shapeOffset // offset of subshape relative to this shape
	)
	{
		m_subShapes.push_back( pShape );
		pShape->SetOffset( shapeOffset );
	}

private:
	PixelPoint      m_shapeOffset;    // offset relative to superior shape
	vector<Shape *> m_subShapes;
};
