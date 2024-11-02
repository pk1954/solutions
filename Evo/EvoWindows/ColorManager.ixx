// ColorManager.h
//
// EvoWindows

export module ColorManager;

import std;
import Debug;
import BoolOp;
import SimpleColLUT;
import EvoCoreLib;
import WinBasics;

using std::array;

export class ColorManager
{
public:
	void  Initialize();
	void  ColorDialog(HWND const, tColorObject const, Strategy::Id const = Strategy::Id::empty);
	Color GetColor(tColorObject const, Strategy::Id const = Strategy::Id::empty, ColIndex const = STRATEGY_COLOR());
	void  SetColor(Color const, tColorObject const, Strategy::Id const = Strategy::Id::empty);
	void  ToggleClutMode();

	static ColIndex const STRATEGY_COLOR()
	{
		static ColIndex constexpr res(-1);
		return res;
	}

private:
	void     setupClut(tBoolOp const);

    bool  m_bDimmIndividuals;
	Color m_colorSelection;
	Color m_colorHighlight;
	
	array<SimpleColLUT, Strategy::COUNT> m_aClutStrat;

	SimpleColLUT & getClut(Strategy::Id const strat)
	{
		int iIndex = static_cast<int>(strat);
		AssertLimits(iIndex, 0, Strategy::COUNT - 1);
		return m_aClutStrat[iIndex];
	}

	Color getStrategyColor(Strategy::Id const strat)
	{
		return getClut(strat).GetColorHi();
	}

	void setStrategyColor(Strategy::Id const strat, COLORREF const col)
	{
		getClut(strat).SetColorHi(col);
	}
};