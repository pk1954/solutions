// win32_coordShape.h
//

#pragma once

#include "windows.h"
#include "Resource.h"
#include "win32_shape.h"

class CoordShape : public Shape
{
public:
	CoordShape( TextDisplay & t ) :
		Shape( t )
	{}

	void FillBuffer( GridPoint const gp )
	{
		m_textDisplay.Buffer() << gp;
	}

	virtual void AddContextMenuEntries( HMENU const hPopupMenu ) const 
	{
	    UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDD_TOGGLE_COORD_DISPLAY, L"Toggle coord display" );
	};
};
