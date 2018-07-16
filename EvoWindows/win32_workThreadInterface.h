// win32_workThreadInterface.h

#pragma once

#include <fstream>
#include "gridRect.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"

class PerformanceWindow;
class EditorWindow;
class EvolutionCore;
class EvolutionModelData;
class DisplayAll;
class EvoHistorySys;
class WorkThread;

class WorkThreadInterface
{
public:
	explicit WorkThreadInterface( std::wostream * );
    ~WorkThreadInterface( );

    void Start
    ( 
        PerformanceWindow  * const,
		EditorWindow       * const,  
        DisplayAll   const * const, 
        EvolutionCore      * const,
        EvolutionModelData * const,
	    EvoHistorySys      * const
    );

    void PostDoEdit( GridPoint const & );
    void PostSetBrushMode( tBrushMode const );
    void PostSetBrushShape( tShape );
    void PostSetBrushIntensity( INT const );
    void PostSetBrushSize( INT const );
    void PostRefresh( );
    void PostReset( );
    void PostEndThread( HWND );
    void PostProcessScript( std::wstring const & );
	void PostRunGenerations();
	void PostStopComputation();
	void PostUndo();
	void PostRedo();
	void PostPrevGeneration();
	void PostGotoGeneration( HIST_GENERATION const );
	void PostHistoryAction( UINT const, GridPoint const );
	void PostGenerationStep();
	void DoProcessScript( std::wstring * const ); // parameter must be created with new, will be deleted here! 

	HIST_GENERATION GetGenDemanded( ) const;

private:
	void postGotoGeneration( HIST_GENERATION const );

    EvolutionModelData * m_pModelWork;
    EvoHistorySys      * m_pEvoHistorySys;
	WorkThread         * m_pWorkThread;
    std::wostream      * m_pTraceStream;
	BOOL                 m_bTrace;
}; 
