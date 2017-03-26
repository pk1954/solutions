// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "EvoGenerationCmd.h"
#include "win32_worker_thread.h"

class EvolutionModelData;

class EvoModelData: public ModelData
{
public:
	
    EvoModelData( EvolutionCore * const pCore, EvolutionModelData * pModelDataWork, WorkThread * pWorkThread ):
		m_pEvolutionCore( pCore ),
        m_pEvolutionModelData( pModelDataWork ),
		m_pWorkThread( pWorkThread )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( );

	virtual void OnNextGeneration()	{ m_pEvolutionCore->Compute( m_pEvolutionModelData ); }
	virtual void OnReset()	        { m_pWorkThread->WorkThread::ResetModel( ); }  // Layer 2
    virtual void OnAppCommand( unsigned short, short );
	virtual void CopyModelData( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionCore      * m_pEvolutionCore;
	EvolutionModelData * m_pEvolutionModelData;
	WorkThread         * m_pWorkThread;
    GridPoint            m_gpEdit;
};
