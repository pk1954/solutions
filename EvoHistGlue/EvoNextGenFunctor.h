// EvoNextGenFunctor.h

#pragma once

#include "NextGenFunctor.h"
#include "win32_worker_thread.h"

class EvolutionCore;

class EvoNextGenFunctor : public NextGenFunctor
{
public:
    EvoNextGenFunctor( ) : m_pWorkThread( nullptr ) 
    { }

    void Start( WorkThread * const pWorkThread )
    {
        m_pWorkThread = pWorkThread;
    }

	virtual void OnNextGeneration() const
	{
        m_pWorkThread->GenerationStep( );
	}

	virtual void OnReset() const
	{
        m_pWorkThread->ResetModel( );
	}

    virtual void OnAppCommand ( unsigned short usCmd, short sParam  ) const
    {
        m_pWorkThread->WorkThread::ApplyEditorCommand
		( 
			static_cast<tEvoCmd>( usCmd ), sParam
		);
    }

private:

    WorkThread * m_pWorkThread;
};
