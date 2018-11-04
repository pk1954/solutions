// win32_rectShape.h : 
//
// RectShape is defined by the offset of the top left corner (inherited from Shape)
// and the size of the rectangle 

#pragma once

#include "pixelRect.h"
#include "pixelPoint.h"
#include "win32_shape.h"

using namespace std;

class RectShape : public Shape
{
public:
	RectShape( Shape * const pParent ) :
		Shape( pParent )
	{}

	PixelRect const GetRect( ) const 
	{
		return PixelRect( GetAbsoluteOffset(), m_rectSize );
	}

	virtual bool PointInShape( PixelPoint const pnt ) const
	{
		return GetRect( ).Includes( pnt );
	}

	PixelRectSize const GetSize() const 
	{
		return m_rectSize;
	}

	void SetSize( PixelRectSize const rectSize )
	{
		m_rectSize = rectSize;
	}

private:
	PixelRectSize m_rectSize;
};
