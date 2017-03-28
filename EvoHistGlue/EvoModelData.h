// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "ModelData.h"
#include "win32_worker_thread.h"

class EvolutionModelData;

class EvoModelData: public ModelData
{
public:
	
    EvoModelData( EvolutionModelData * pModelDataWork, WorkThread * pWorkThread ):
        m_pEvolutionModelData( pModelDataWork ),
		m_pWorkThread( pWorkThread )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( );

	virtual void OnNextGeneration()	{ m_pWorkThread->WorkThread::GenerationStep( ); }   // Layer 2
	virtual void OnReset()	        { m_pWorkThread->WorkThread::ResetModel( ); }       // Layer 2
    virtual void OnAppCommand( unsigned short, short );
	virtual void CopyModelData( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionModelData * m_pEvolutionModelData;
	WorkThread         * m_pWorkThread;
    GridPoint            m_gpEdit;
};
