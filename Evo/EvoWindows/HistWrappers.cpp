// HistWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//
// EvoWindows

module HistWrappers;

import Resource;
import RunTime;

static WorkThreadInterface * m_pWorkThreadInterface;

class WrapPostGotoGeneration : public ScriptFunctor
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

    SymbolTable::ScrDefConst(L"PostGotoGeneration", new WrapPostGotoGeneration);

	DEF_ULONG_CONST(IDM_GOTO_ORIGIN);
    DEF_ULONG_CONST(IDM_GOTO_DEATH);
}
