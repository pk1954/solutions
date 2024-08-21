// HistWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//
// EvoWindows

#include "symtab.h"
#include "SCRIPT.H"
#include "win32_WorkThreadInterface.h"

import Resource;

static WorkThreadInterface * m_pWorkThreadInterface;

class WrapPostGotoGeneration : public Script_Functor
{
public:
	virtual void operator() (Script & script) const
	{
		HistGeneration const gen = static_cast<HistGeneration>(script.ScrReadLong());
		m_pWorkThreadInterface->PostGotoGeneration(gen);
	}
};

void DefineWin32HistWrapperFunctions(WorkThreadInterface * pWorkThreadInterface)
{
    m_pWorkThreadInterface = pWorkThreadInterface;

	DEF_FUNC(PostGotoGeneration);

	DEF_ULONG_CONST(IDM_GOTO_ORIGIN);
    DEF_ULONG_CONST(IDM_GOTO_DEATH);
}
