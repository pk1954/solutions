// win32_histWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//
// EvoWindows

#include "stdafx.h"
#include "symtab.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "win32_WorkThreadInterface.h"

static WorkThreadInterface * m_pWorkThreadInterface;

class WrapPostGotoGeneration : public Script_Functor
{
public:
	virtual void operator() ( Script & script ) const
	{
		HIST_GENERATION const gen = static_cast<HIST_GENERATION>( script.ScrReadLong( ) );
		m_pWorkThreadInterface->PostGotoGeneration( gen );
	}
};

void DefineWin32HistWrapperFunctions( WorkThreadInterface * pWorkThreadInterface )
{
    m_pWorkThreadInterface = pWorkThreadInterface;

	DEF_FUNC( PostGotoGeneration );

	DEF_ULONG_CONST( IDM_GOTO_ORIGIN );
    DEF_ULONG_CONST( IDM_GOTO_DEATH );
}
