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

    void            PostGotoGeneration( HIST_GENERATION const );
    HIST_GENERATION GetGenDemanded( )       const { return m_genDemanded; }
    HIST_GENERATION GetCurrentGeneration( ) const { return m_pEvoHistorySys->GetCurrentGeneration( ); }

    EvoHistorySys * GetEvoHistorySys( ) { return m_pEvoHistorySys; }

	// overwrite methods from WorkThread

	virtual void StopComputation();
	virtual void PostNextGeneration();
	virtual void PostHistoryAction(UINT const, GridPoint const);
	virtual void ApplyEditorCommand(tEvoCmd const, short const);
	virtual void DoEdit(GridPoint const);
	virtual void DoExit( HWND );

private:

	virtual void generationRun( );

    void generationStep( );
	void gotoGeneration( HIST_GENERATION const );

    HIST_GENERATION  m_genDemanded;
    EvoHistorySys  * m_pEvoHistorySys;
    EvolutionCore  * m_pCore;
    ModelData      * m_pModelWork;
};
