// win32_workThreadInterface.h

#pragma once

#include <fstream>
#include "boolOp.h"
#include "gridRect.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"

class PerformanceWindow;
class EditorWindow;
class EvolutionCore;
class EventInterface;
class ObserverInterface;
class EvoHistorySysGlue;
class WorkThread;

class WorkThreadInterface
{
public:
	explicit WorkThreadInterface( std::wostream * );
    ~WorkThreadInterface( );

    void Start
    ( 
        PerformanceWindow * const,
		EditorWindow      * const,  
		EventInterface    * const,
		ObserverInterface * const, 
        EvolutionCore     * const,
	    EvoHistorySysGlue * const
    );

    void PostDoEdit( GridPoint const );
    void PostSetPOI( GridPoint const );
    void PostSetSimulationMode( tBoolOp const );
    void PostSetBrushMode( tBrushMode const );
    void PostSetBrushShape( tShape );
	void PostSetBrushManipulator( tManipulator const );
    void PostSetBrushIntensity( LONG const );
    void PostSetBrushRadius( INT const );
    void PostRefresh( LPARAM const );
    void PostReset( BOOL );
    void PostProcessScript( std::wstring const & );
	void PostRunGenerations( bool const bFirst = true );
	void PostStopComputation();
	void PostUndo();
	void PostRedo();
	void PostPrevGeneration();
	void PostGotoGeneration( HIST_GENERATION const );
	void PostHistoryAction( UINT const, GridPoint const );
	void PostGenerationStep();
	void PostRepeatGenerationStep();              // Do not call! Used by WorkThread only;
	void DoProcessScript( std::wstring * const ); // parameter must be created with new, will be deleted here! 

	HIST_GENERATION GetGenDemanded( ) const;

    void TerminateThread( );

private:
	void postGotoGeneration( HIST_GENERATION const );

    EvolutionCore     * m_pCore;
    EvoHistorySysGlue * m_pEvoHistGlue;
	WorkThread        * m_pWorkThread;
    std::wostream     * m_pTraceStream;
	BOOL                m_bTrace;
}; 
