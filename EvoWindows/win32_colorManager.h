// win32_colorManager.h
//

#pragma once

#include <array>
#include "strategy.h"
#include "win32_clut.h"

class ColorManager
{
public:
	void     Start();
	void     ColorDialog( HWND const, tColorObject const, Strategy::Id const = Strategy::Id::empty );
	COLORREF GetColor( tColorObject const, Strategy::Id const = Strategy::Id::empty, CLUT_INDEX const = STRATEGY_COLOR() );
	void     SetColor( COLORREF const, tColorObject const, Strategy::Id const = Strategy::Id::empty );
	void     ToggleClutMode( );

	static CLUT_INDEX const STRATEGY_COLOR()
	{
		static const CLUT_INDEX res(-1);
		return res;
	}

private:
	void     setupClut( tBoolOp const );

    bool     m_bDimmIndividuals;
	COLORREF m_colorSelection;
	COLORREF m_colorHighlight;
	
	std::array< CLUT, Strategy::COUNT> m_aClutStrat;

	CLUT & getClut(Strategy::Id const strat)
	{
		int iIndex = static_cast<int>(strat);
		AssertLimits( iIndex, 0, Strategy::COUNT - 1 );
		return m_aClutStrat[ iIndex ];
	}

	COLORREF getStrategyColor( Strategy::Id const strat )
	{
		return getClut( strat ).GetColorHi( );
	}

	void setStrategyColor( Strategy::Id const strat, COLORREF const col )
	{
		getClut( strat ).SetColorHi( col );
	}
};