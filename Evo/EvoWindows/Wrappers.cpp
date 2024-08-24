// Wrappers.cpp : wrapper functions for unit tests and scripting of application
//
// EvoWindows

#include"EvolutionCoreWrapperHelpers.h"
#include"win32_EvoWorkThreadInterface.h"
#include"win32_wrappers.h"

import RunTime;
import Resource;

module EvoGenerationCmd;

static EvoWorkThreadInterface * m_pWorkThreadInterface;

class WrapPostPrevGeneration : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostPrevGeneration();
    }
};

class WrapPostBenchmark : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
		int iNrOfGenerations = script.ScrReadInt();
        m_pWorkThreadInterface->PostBenchmark(iNrOfGenerations);
    }
};

class WrapPostDoEdit : public ScriptFunctor
{
public:
	virtual void operator() (Script & script) const
	{
		GridPoint gp = ScrReadGridPoint(script);
		m_pWorkThreadInterface->PostDoEdit(gp);
	}
};

class WrapPostSetPOI : public ScriptFunctor
{
public:
	virtual void operator() (Script & script) const
	{
		GridPoint gp = ScrReadGridPoint(script);
		m_pWorkThreadInterface->PostSetPOI(gp);
	}
};

class WrapPostSetBrushMode : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        ULONG      const ulCmd = script.ScrReadUlong();
        tBrushMode const mode  = static_cast<tBrushMode>(ulCmd);
        m_pWorkThreadInterface->PostSetBrushMode(mode);
    }
};

class WrapPostSetBrushShape : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        ULONG  const ulShape = script.ScrReadUlong();
        tShape const shape   = static_cast<tShape>(ulShape);
        m_pWorkThreadInterface->PostSetBrushShape(shape);
    }
};

class WrapPostSetBrushIntensity : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
		PERCENT const intensity { script.ScrReadShort() };
        m_pWorkThreadInterface->PostSetBrushIntensity(intensity);
    }
};

class WrapPostSetBrushRadius : public ScriptFunctor
{
public:
	virtual void operator() (Script & script) const
	{
		GridCoord const size = ScrReadGridCoord(script);
		m_pWorkThreadInterface->PostSetBrushRadius(size);
	}
};

class WrapPostSetBrushManipulator : public ScriptFunctor
{
public:
	virtual void operator() (Script & script) const
	{
		USHORT       const usValue     = script.ScrReadUshort();
		tManipulator const manipulator = static_cast<tManipulator>(usValue);
		m_pWorkThreadInterface->PostSetBrushManipulator(manipulator);
	}
};

class WrapPostGenerationStep : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostGenerationStep();
    }
};

class WrapPostRunGenerations : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostGenerationStep();
    }
};

class WrapBreak : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
    }
};

void DefineWin32WrapperFunctions(EvoWorkThreadInterface * const pWorkThreadInterface)
{
    m_pWorkThreadInterface = pWorkThreadInterface;

    SymbolTable::ScrDefConst(L"PostGotoGeneration",      new WrapPostGotoGeneration);
    SymbolTable::ScrDefConst(L"PostPrevGeneration",      new WrapPostPrevGeneration); 
	SymbolTable::ScrDefConst(L"PostBenchmark",           new WrapPostBenchmark);
	SymbolTable::ScrDefConst(L"PostDoEdit",              new WrapPostDoEdit);
	SymbolTable::ScrDefConst(L"PostSetPOI",              new WrapPostSetPOI);
    SymbolTable::ScrDefConst(L"PostSetBrushMode",        new WrapPostSetBrushMode);
    SymbolTable::ScrDefConst(L"PostSetBrushShape",       new WrapPostSetBrushShape);
    SymbolTable::ScrDefConst(L"PostSetBrushIntensity",   new WrapPostSetBrushIntensity);
    SymbolTable::ScrDefConst(L"PostSetBrushRadius",      new WrapPostSetBrushRadius);
	SymbolTable::ScrDefConst(L"PostSetBrushManipulator", new WrapPostSetBrushManipulator);
    SymbolTable::ScrDefConst(L"PostGenerationStep",      new WrapPostGenerationStep);
    SymbolTable::ScrDefConst(L"PostRunGenerations",      new WrapPostRunGenerations);
    SymbolTable::ScrDefConst(L"Break",                   new WrapBreak);

    DEF_ULONG_CONST(IDM_FORWARD);
    DEF_ULONG_CONST(IDM_BACKWARDS);
    DEF_ULONG_CONST(IDM_RUN);
    DEF_ULONG_CONST(IDM_STOP);

    DEF_ULONG_CONST(EvoGenerationCmd::Id::editSetBrushMode);
    DEF_ULONG_CONST(EvoGenerationCmd::Id::editSetBrushShape);
    DEF_ULONG_CONST(EvoGenerationCmd::Id::editSetBrushRadius);
    DEF_ULONG_CONST(EvoGenerationCmd::Id::editSetBrushIntensity);
    DEF_ULONG_CONST(EvoGenerationCmd::Id::editDoEdit);
    DEF_ULONG_CONST(EvoGenerationCmd::Id::editSetPOI);
}
