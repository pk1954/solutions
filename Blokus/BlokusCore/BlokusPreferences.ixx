// BlokusPreferences.ixx
//
// BlokusCore

export module BlokusCore:BlokusPreferences;

import IoUtil;
import BoolType;
import BoolPreferences;
import Win32_Util_Resource;

export class BlokusPreferences
{
public:
	static void Initialize()
    {
        Preferences::Initialize(L"Blokus_UserPreferences.txt");

		m_bPrefShowPieceNumbers = BoolPreferences::CreateNew(L"ShowPieceNumbers");
		m_bPrefShowContactPnts  = BoolPreferences::CreateNew(L"ShowContactPnts" );
		m_bPrefShowCellNumbers  = BoolPreferences::CreateNew(L"ShowCellNumbers" );
		m_bPrefShowBlockedCells = BoolPreferences::CreateNew(L"ShowBlockedCells");
		m_bPrefShowCornerCells  = BoolPreferences::CreateNew(L"ShowCornerCells" );
		m_bPrefShowMoveDetail   = BoolPreferences::CreateNew(L"ShowMoveDetail"  );
		m_bPrefShowAnimation    = BoolPreferences::CreateNew(L"ShowAnimation"   );
		m_bPrefSound            = BoolPreferences::CreateNew(L"Sound"           );
    }

	inline static IdBoolPref m_bPrefShowPieceNumbers;
	inline static IdBoolPref m_bPrefShowContactPnts;
	inline static IdBoolPref m_bPrefShowCellNumbers;
	inline static IdBoolPref m_bPrefShowBlockedCells;
	inline static IdBoolPref m_bPrefShowCornerCells;
	inline static IdBoolPref m_bPrefShowMoveDetail;
	inline static IdBoolPref m_bPrefShowAnimation;
	inline static IdBoolPref m_bPrefSound;

private:

};
