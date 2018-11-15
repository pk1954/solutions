// win32_colorManager.h
//

#pragma once

#include <array>
#include "Windows.h"
#include "Commdlg.h"
#include "EvolutionTypes.h"
#include "win32_clut.h"

class ColorManager
{
public:
	enum class tObject : unsigned short
	{
		individual,
		selection,
		highlight
	};

	ColorManager();
	
	void     ColorDialog( HWND const, tObject const, tStrategyId const = tStrategyId::empty );
	COLORREF GetColor( tObject const, tStrategyId const = tStrategyId::empty, UINT const = -1 );
	void     SetColor( COLORREF const, tObject const, tStrategyId const = tStrategyId::empty );
	void     ToggleClutMode( );

private:
	COLORREF getStrategyColor( tStrategyId const );
	void     setStrategyColor( tStrategyId const, COLORREF const );
	void     setupClut( tBoolOp const );

    bool     m_bDimmIndividuals;
	COLORREF m_colorSelection;
	COLORREF m_colorHighlight;
	array< CLUT, NR_STRATEGIES> m_aClutStrat;
};