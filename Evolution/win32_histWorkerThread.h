// win32_histWorkerThread.h

#pragma once

#include "HistoryGeneration.h"
#include "win32_worker_thread.h"
#include "EvoHistorySys.h"

class EvoHistorySys;
class EvolutionCore;
class ModelData;

class HistWorkThread : public WorkThread
{
public:
    HistWorkThread( wofstream *, EvolutionCore * const, ModelData * const, EvoHistorySys * const );
    ~HistWorkThread( );

    void            PostAllocateHistorySlots( );
    void            PostGotoGeneration( HIST_GENERATION const );
    HIST_GENERATION GetGenDemanded( )       const { return m_genDemanded; }
    HIST_GENERATION GetCurrentGeneration( ) const { return m_pEvoHistorySys->GetCurrentGeneration( ); }

    EvoHistorySys * GetEvoHistorySys( )       { return m_pEvoHistorySys; }
    BOOL            ContinueSlotAllocation( ) { return m_bContinueSlotAllocation; }

	// overwrite methods from WorkThread

	virtual void    PostHistoryAction( UINT const, GridPoint const );
	virtual void    PostNextGeneration();
	virtual void    ApplyEditorCommand(tEvoCmd const, short const);

private:
    enum tHistThreadMessages
    {
        THREAD_MSG_HIST_GOTO_GEN = THREAD_MSG_LAST + 1,
        THREAD_MSG_HIST_GOTO_DEM,                                // goto generation m_genDemanded
        THREAD_MSG_HIST_FIRST    = THREAD_MSG_HIST_GOTO_GEN,
        THREAD_MSG_HIST_LAST     = THREAD_MSG_HIST_GOTO_DEM
    };

    virtual DWORD processWorkerMessage( UINT, WPARAM, LPARAM );
    virtual void  generationRun( );
    virtual void  generationStep( );

    HIST_GENERATION  m_genDemanded;
    EvoHistorySys  * m_pEvoHistorySys;
    EvolutionCore  * m_pCore;
    ModelData      * m_pModelWork;
    HANDLE           m_hThreadSlotAllocator;
    BOOL             m_bContinueSlotAllocation;
};
