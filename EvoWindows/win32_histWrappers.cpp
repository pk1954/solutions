// win32_histWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//

#include "stdafx.h"
#include "symtab.h"
#include "SCRIPT.H"
#include "config.h"
#include "Resource.h"
#include "win32_EvoWorkThreadInterface.h"

static EvoWorkThreadInterface * m_pWorkThreadInterface;

void DefineWin32HistWrapperFunctions( EvoWorkThreadInterface * pWorkThreadInterface )
{
    m_pWorkThreadInterface = pWorkThreadInterface;

    DEF_ULONG_CONST( IDM_GOTO_ORIGIN );
    DEF_ULONG_CONST( IDM_GOTO_DEATH );
}
