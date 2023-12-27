// win32_wrappers.cpp : wrapper functions for unit tests and scripting of application
//


#include "symtab.h"
#include "Resource.h"
#include "EvolutionCoreWrapperHelpers.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_wrappers.h"

module EvoGenerationCmd;

static EvoWorkThreadInterface * m_pWorkThreadInterface;

class WrapPostPrevGeneration : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostPrevGeneration();
    }
};

class WrapPostBenchmark : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
		int iNrOfGenerations = script.ScrReadInt();
        m_pWorkThreadInterface->PostBenchmark(iNrOfGenerations);
    }
};

class WrapPostDoEdit : public Script_Functor
{
public:
	virtual void operator() (Script & script) const
	{
		GridPoint gp = ScrReadGridPoint(script);
		m_pWorkThreadInterface->PostDoEdit(gp);
	}
};

class WrapPostSetPOI : public Script_Functor
{
public:
	virtual void operator() (Script & script) const
	{
		GridPoint gp = ScrReadGridPoint(script);
		m_pWorkThreadInterface->PostSetPOI(gp);
	}
};

class WrapPostSetBrushMode : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
        ULONG      const ulCmd = script.ScrReadUlong();
        tBrushMode const mode  = static_cast<tBrushMode>(ulCmd);
        m_pWorkThreadInterface->PostSetBrushMode(mode);
    }
};

class WrapPostSetBrushShape : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
        ULONG  const ulShape = script.ScrReadUlong();
        tShape const shape   = static_cast<tShape>(ulShape);
        m_pWorkThreadInterface->PostSetBrushShape(shape);
    }
};

class WrapPostSetBrushIntensity : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
		PERCENT const intensity { script.ScrReadShort() };
        m_pWorkThreadInterface->PostSetBrushIntensity(intensity);
    }
};

class WrapPostSetBrushRadius : public Script_Functor
{
public:
	virtual void operator() (Script & script) const
	{
		GridCoord const size = ScrReadGridCoord(script);
		m_pWorkThreadInterface->PostSetBrushRadius(size);
	}
};

class WrapPostSetBrushManipulator : public Script_Functor
{
public:
	virtual void operator() (Script & script) const
	{
		USHORT       const usValue     = script.ScrReadUshort();
		tManipulator const manipulator = static_cast<tManipulator>(usValue);
		m_pWorkThreadInterface->PostSetBrushManipulator(manipulator);
	}
};

class WrapPostGenerationStep : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostGenerationStep();
    }
};

class WrapPostRunGenerations : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pWorkThreadInterface->PostGenerationStep();
    }
};

class WrapBreak : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
    }
};

void DefineWin32WrapperFunctions(EvoWorkThreadInterface * const pWorkThreadInterface)
{
    m_pWorkThreadInterface = pWorkThreadInterface;

    DEF_FUNC(PostPrevGeneration); 

	DEF_FUNC(PostBenchmark);
	DEF_FUNC(PostDoEdit);
	DEF_FUNC(PostSetPOI);
    DEF_FUNC(PostSetBrushMode);
    DEF_FUNC(PostSetBrushShape);
    DEF_FUNC(PostSetBrushIntensity);
    DEF_FUNC(PostSetBrushRadius);
	DEF_FUNC(PostSetBrushManipulator);
    DEF_FUNC(PostGenerationStep);
    DEF_FUNC(PostRunGenerations);
    DEF_FUNC(Break);

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
