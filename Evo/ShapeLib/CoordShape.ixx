// CoordShape.ixx
//
// ShapeLib

export module ShapeLib:CoordShape;

//import Resource;
import WinBasics;
import EvoCoreLib;
import :Shape;

export class CoordShape : public Shape
{
public:
	void FillBuffer(EvolutionCore const* const pCore, GridPoint const gp)
	{
		if (EvoConfig::GetConfigValueBool(EvoConfig::tId::showGridPointCoords))
			m_pTextDisplay->Buffer() << gp;
	}

	virtual void AddContextMenuEntries(HMENU const hPopupMenu) const
	{
		UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;
		//AppendMenuW(hPopupMenu, STD_FLAGS, IDD_TOGGLE_COORD_DISPLAY, L"Toggle coord display");
		// TODO
	};
};
