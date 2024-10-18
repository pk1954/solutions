// BlokusPreferences.ixx
//
// BlokusCore

export module BlokusCore:BlokusPreferences;

import BoolType;
import IoUtil;

export class BlokusPreferences
{
public:
	static void Initialize()
    {
        Preferences::Initialize(L"Blokus_UserPreferences.txt");

        Preferences::AddBoolWrapper(L"ShowContactPnts", m_bShowContactPnts);
        Preferences::AddBoolWrapper(L"ShowCornerCells", m_bShowCornerCells);
        Preferences::AddBoolWrapper(L"ShowAnimation"  , m_bShowAnimation);
    }

	inline static BoolType m_bShowContactPnts { true };
	inline static BoolType m_bShowCornerCells { true };
	inline static BoolType m_bShowAnimation   { true };

private:

};
