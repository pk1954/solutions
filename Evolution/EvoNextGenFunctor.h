// EvoNextGenFunctor.h

#pragma once

#include "NextGenFunctor.h"
#include "win32_worker_thread.h"

class EvolutionCore;

class EvoNextGenFunctor : public NextGenFunctor
{
public:
    EvoNextGenFunctor( ) :
        m_pWorkThread( nullptr )
    { }

    void Start
    ( 
        WorkThread * const pWorkThread
    )
    {
        m_pWorkThread = pWorkThread;
    }

    // operator( ) - apply GenerationCmd to model
    //               compute next generation or
    //               reset model or
    //               or apply editor command

    virtual void operator() ( GenerationCmd genCmd ) const
    {
        tGenCmd const cmd = genCmd.GetCommand( );

        switch ( cmd )
        {
        case tGenCmd::nextGen:
            m_pWorkThread->WorkThread::GenerationStep( );
            break;

        case tGenCmd::reset:
            m_pWorkThread->ResetModel( );
            break;

        case tGenCmd::undefined:
        case tGenCmd::cached:
            assert( false );

        default:
            m_pWorkThread->ApplyEditorCommand( static_cast<tEvoCmd>(genCmd.GetCommand( )), genCmd.GetParam( ) );
        }
    }

private:

    WorkThread * m_pWorkThread;
};
