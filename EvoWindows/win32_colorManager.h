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
	ColorManager();
	
	void  ColorDialog( HWND const, tStrategyId const );
	DWORD GetColorFromClut( tStrategyId const, UINT const );
	void  ToggleClutMode( );

private:
	COLORREF getStrategyColor( tStrategyId const );
	void     setStrategyColor( tStrategyId const, COLORREF const );
	void     setupClut( tBoolOp const );

    bool m_bDimmIndividuals;   
	array< CLUT, NR_STRATEGIES> m_aClutStrat;
};