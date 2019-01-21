// win32_colorManager.h
//

#pragma once

#include <array>
#include "EvolutionTypes.h"
#include "win32_clut.h"

class ColorManager
{
public:
	ColorManager();
	
	void     ColorDialog( HWND const, tColorObject const, tStrategyId const = tStrategyId::empty );
	COLORREF GetColor( tColorObject const, tStrategyId const = tStrategyId::empty, UINT const = -1 );
	void     SetColor( COLORREF const, tColorObject const, tStrategyId const = tStrategyId::empty );
	void     ToggleClutMode( );

private:
	void     setupClut( tBoolOp const );

    bool     m_bDimmIndividuals;
	COLORREF m_colorSelection;
	COLORREF m_colorHighlight;
	
	array< CLUT, NR_STRATEGIES> m_aClutStrat;

	CLUT & getClut(tStrategyId const strat)
	{
		int iIndex = static_cast<int>(strat);
		AssertLimits( iIndex, 0, NR_STRATEGIES - 1 );
		return m_aClutStrat[ iIndex ];
	}

	COLORREF getStrategyColor( tStrategyId const strat )
	{
		return getClut( strat ).GetColorHi( );
	}

	void setStrategyColor( tStrategyId const strat, COLORREF const col )
	{
		getClut( strat ).SetColorHi( col );
	}
};