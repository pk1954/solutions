// win32_workThreadInterface.h

#pragma once

#include <fstream>
#include "boolOp.h"
#include "gridRect.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"
#include "win32_colorManager.h"

class Delay;
class ActionTimer;
class ObserverInterface;
class EvolutionCore;
class EventInterface;
class ReadBuffer;
class EvoHistorySysGlue;
class WorkThread;

class WorkThreadInterface
{
public:
	WorkThreadInterface( );
    ~WorkThreadInterface( );

	void Initialize( std::wostream * );

	void Start
    ( 
		HWND                const,
		ColorManager      * const,
		ActionTimer       * const,
		EventInterface    * const,
		Delay             * const,
		ReadBuffer        * const, 
	    EvoHistorySysGlue * const
    );

	void Stop( );

	void RegisterRunObserver( ObserverInterface * const );

    void PostDoEdit( GridPoint const );
    void PostSetPOI( GridPoint const );
    void PostSetBrushMode( tBrushMode const );
    void PostSetBrushShape( tShape );
	void PostSetBrushManipulator( tManipulator const );
    void PostSetBrushIntensity( PERCENT const );
    void PostSetColor( COLORREF const, tColorObject const, Strategy::Id const );
    void PostSetBrushRadius( GRID_COORD const );
    void PostReset( BOOL );
	void PostRunGenerations( bool const );
	void PostStopComputation();
	void PostUndo();
	void PostRedo();
	void PostBenchmark( int const );
	void PostPrevGeneration();
	void PostGotoGeneration( HIST_GENERATION const );
	void PostGotoOrigin( GridPoint const );
	void PostGotoDeath ( GridPoint const );
	void PostGenerationStep();
	void PostRepeatGenerationStep();              // Do not call! Used by WorkThread only;

	HIST_GENERATION GetGenDemanded( ) const;
	BOOL            IsRunning( )      const;

    void TerminateThread( );

private:
	void postGotoGeneration( HIST_GENERATION const );

    EvoHistorySysGlue * m_pEvoHistGlue;
	WorkThread        * m_pWorkThread;
    std::wostream     * m_pTraceStream;
	BOOL                m_bTrace;
}; 
