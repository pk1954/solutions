// win32_shape.h : 
//

#pragma once

#include "vector"
#include "pixelPoint.h"

using namespace std;

class Shape
{
public:
	Shape( Shape * const pParent ) :
		m_pParent( pParent ),
		m_shapeOffset( PixelPoint( 0, 0 ) )
	{}

	virtual bool PointInShape( PixelPoint const & ) const = 0;

	PixelPoint GetAbsoluteOffset( ) const
	{
		PixelPoint pRes = m_shapeOffset;
		if ( m_pParent )
			pRes += m_pParent->GetAbsoluteOffset();
		return pRes;
	}

	void SetShapeOffset( PixelPoint const offset )
	{
		m_shapeOffset = offset;
	}

private:
	PixelPoint m_shapeOffset;    // offset relative to superior shape
	Shape    * m_pParent;
};
