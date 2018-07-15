// win32_histWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//

#include "stdafx.h"
#include "symtab.h"
#include "SCRIPT.H"
#include "config.h"
#include "Resource.h"
#include "win32_worker_thread.h"

static WorkThread * m_pWorkThread;

class WrapPostGotoGeneration : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        HIST_GENERATION const gen = static_cast<HIST_GENERATION>( script.ScrReadLong( ) );
        if ( Config::UseHistorySystem( ) )
            m_pWorkThread->PostGotoGeneration( gen );
    }
};

void DefineWin32HistWrapperFunctions( WorkThread * pWorkThread )
{
    m_pWorkThread = pWorkThread;

    DEF_FUNC( PostGotoGeneration );

    DEF_ULONG_CONST( IDM_GOTO_ORIGIN );
    DEF_ULONG_CONST( IDM_GOTO_DEATH );
}
