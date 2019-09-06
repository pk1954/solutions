// win32_EvoWorkThreadInterface.h
//
// EvoWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "gridRect.h"
#include "strategy.h"
#include "EvolutionTypes.h"
#include "win32_WorkThreadInterface.h"

class ColorManager;
class ActionTimer;
class EventInterface;
class Delay;
class ObserverInterface;
class EvoHistorySysGlue;
class EvoWorkThread;

class EvoWorkThreadInterface : public WorkThreadInterface
{
public:
	EvoWorkThreadInterface( );
    ~EvoWorkThreadInterface( );

	void Start
    ( 
		HWND                const,
		ColorManager      * const,
		ActionTimer       * const,
		EventInterface    * const,
		Delay             * const,
		ObserverInterface * const, 
	    EvoHistorySysGlue * const
    );

    void PostDoEdit( GridPoint const );
    void PostSetPOI( GridPoint const );
    void PostSetBrushMode( tBrushMode const );
    void PostSetBrushShape( tShape );
	void PostSetBrushManipulator( tManipulator const );
    void PostSetBrushIntensity( PERCENT const );
    void PostSetColor( COLORREF const, tColorObject const, Strategy::Id const );
    void PostSetBrushRadius( GRID_COORD const );
    void PostReset( BOOL );
	void PostUndo();
	void PostRedo();
	void PostBenchmark( int const );
	void PostGotoOrigin( GridPoint const );
	void PostGotoDeath ( GridPoint const );

private:
	EvoWorkThread * m_pEvoWorkThread;
}; 
