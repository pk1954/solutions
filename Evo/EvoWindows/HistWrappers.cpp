// HistWrappers.cpp : wrapper functions for unit tests and scripting of win32_history library
//
// EvoWindows

module HistWrappers;

import Resource;
import RunTime;

static WorkThread * m_pWorkThread;

class WrapPostGotoGeneration : public ScriptFunctor
{
public:
	virtual void operator() (Script & script) const
	{
		HistGeneration const gen = static_cast<HistGeneration>(script.ScrReadLong());
		m_pWorkThread->PostGotoGeneration(gen);
	}
};

void DefineWin32HistWrapperFunctions(WorkThread * pWorkThread)
{
    m_pWorkThread = pWorkThread;

    SymbolTable::ScrDefConst(L"PostGotoGeneration", new WrapPostGotoGeneration);

	DEF_ULONG_CONST(IDM_GOTO_ORIGIN);
    DEF_ULONG_CONST(IDM_GOTO_DEATH);
}
