// BlokusPreferences.ixx
//
// BlokusCore

export module BlokusCore:BlokusPreferences;

import IoUtil;
import BoolType;
import BoolPreferences;
import WinBasics;
import Win32_Util_Resource;

export class BlokusPreferences
{
public:
	static void Initialize
	(
		HMENU hMenuView,
		HMENU hMenuOptions
	)
    {
        Preferences::Initialize(L"Blokus_UserPreferences.txt");

		m_bPrefShowPieceNumbers = BoolPreferences::CreateNew(L"ShowPieceNumbers", hMenuView,    L"Piece numbers"  );
		m_bPrefShowContactPnts  = BoolPreferences::CreateNew(L"ShowContactPnts",  hMenuView,    L"Contact points" );
		m_bPrefShowCellNumbers  = BoolPreferences::CreateNew(L"ShowCellNumbers",  hMenuView,    L"Cell numbers"   );
		m_bPrefShowBlockedCells = BoolPreferences::CreateNew(L"ShowBlockedCells", hMenuView,    L"Blocked cells"  );
		m_bPrefShowCornerCells  = BoolPreferences::CreateNew(L"ShowCornerCells",  hMenuView,    L"Corner cells"   );
		m_bPrefShowMoveDetail   = BoolPreferences::CreateNew(L"ShowMoveDetail",   hMenuView,    L"Move details"   );

		m_bPrefAutoMove         = BoolPreferences::CreateNew(L"AutoMove",         hMenuOptions, L"Automatic moves");
		m_bPrefShowAnimation    = BoolPreferences::CreateNew(L"ShowAnimation",    hMenuOptions, L"&Animation"     );
		m_bPrefSound            = BoolPreferences::CreateNew(L"Sound",            hMenuOptions, L"&Sound"         );
    }

	inline static IdBoolPref m_bPrefShowPieceNumbers;
	inline static IdBoolPref m_bPrefShowContactPnts;
	inline static IdBoolPref m_bPrefShowCellNumbers;
	inline static IdBoolPref m_bPrefShowBlockedCells;
	inline static IdBoolPref m_bPrefShowCornerCells;
	inline static IdBoolPref m_bPrefShowMoveDetail;
	inline static IdBoolPref m_bPrefAutoMove;
	inline static IdBoolPref m_bPrefShowAnimation;
	inline static IdBoolPref m_bPrefSound;
};
