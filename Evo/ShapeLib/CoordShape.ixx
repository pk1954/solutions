// CoordShape.ixx
//
// ShapeLib

module;

#include <windows.h>

export module ShapeLib:CoordShape;

import Config;
import Shape;
import Resource;

export class CoordShape : public Shape
{
public:
	void FillBuffer(EvolutionCore const* const pCore, GridPoint const gp)
	{
		if (Config::GetConfigValueBool(Config::tId::showGridPointCoords))
			m_pTextDisplay->Buffer() << gp;
	}

	virtual void AddContextMenuEntries(HMENU const hPopupMenu) const
	{
		UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;
		(void)AppendMenu(hPopupMenu, STD_FLAGS, IDD_TOGGLE_COORD_DISPLAY, L"Toggle coord display");
	};
};
