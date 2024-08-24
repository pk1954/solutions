// EvolutionCoreWrappers.cpp : wrapper functions for unit tests and scripting of application
//
// EvoCoreLib

module EvoCoreLib:EvolutionCoreWrappers;

import Debug;
import RunTime;
import IoUtil;
import UtilityWrappers;
import :EvolutionTypes;
import :EvolutionCore;
import :EvoPixelCoords;
import :EvolutionCoreWrapperHelpers;

import GridPOI;

static EvolutionCore  * m_pCore;
static EvoPixelCoords * m_pPixCoords;

class WrapSetPoi : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        GridPoint gp = ScrReadGridPoint(script);
        GridPOI::SetPoi(gp);
    }
};

class WrapClearPoi : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        GridPOI::ClearPoi();
    }
};

class WrapDumpGridPointList : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pCore->DumpGridPointList();
    }
};

class WrapCompute : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pCore->Compute();
    }
};

class WrapResetModel : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        m_pCore->ResetAll();
    }
};

class WrapPause : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    { }
};

class WrapModelDoEdit : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        GridPoint gp = ScrReadGridPoint(script);
        m_pCore->ModelDoEdit(gp);
    }
};

class WrapSetBrushShape : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        int    const iShape = script.ScrReadInt();
        tShape const shape  = static_cast<tShape>(iShape);
        m_pCore->SetBrushShape(shape);
    }
};

class WrapSetBrushRadius : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        GridCoord const size = ScrReadGridCoord(script);
        m_pCore->SetBrushRadius(size);
    }
};

class WrapSetBrushIntensity : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        PERCENT const intensity = static_cast<PERCENT>(script.ScrReadShort());
        m_pCore->SetBrushIntensity(intensity);
    }
};

class WrapSetBrushMode : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        int        const iMode = script.ScrReadInt();
        tBrushMode const mode  = static_cast<tBrushMode>(iMode);
        m_pCore->SetBrushMode(mode);
    }
};

class WrapPixel2GridPos : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        PixelPoint const pixPos = ScrReadPixelPoint(script);
		GridPoint  const gp     = m_pPixCoords->Pixel2GridPos(pixPos);
		std::wcout << gp << std::endl;
    }
};

class WrapSetFieldSize : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
		m_pPixCoords->SetGridFieldSize(PIXEL(script.ScrReadShort()));
    }
};

class WrapMoveGrid : public ScriptFunctor
{
public:
    virtual void operator() (Script & script) const
    {
        PixelPoint const pixPosDelta = ScrReadPixelPoint(script);
		m_pPixCoords->MoveGrid(pixPosDelta);
    }
};

void DefineEvoPixelCoordsWrapperFunctions(EvoPixelCoords * pCoords)
{
    m_pPixCoords = pCoords;

    SymbolTable::ScrDefConst(L"Pixel2GridPos", new WrapPixel2GridPos);
    SymbolTable::ScrDefConst(L"SetFieldSize",  new WrapSetFieldSize);
    SymbolTable::ScrDefConst(L"MoveGrid",      new WrapMoveGrid);
}

void DefineCoreWrapperFunctions(EvolutionCore * pCore)
{
	static bool bFirst = true;

    m_pCore = pCore;

	if (bFirst)
	{
		bFirst = false;
        SymbolTable::ScrDefConst(L"Compute",           new WrapCompute);
        SymbolTable::ScrDefConst(L"DumpGridPointList", new WrapDumpGridPointList);
        SymbolTable::ScrDefConst(L"ResetModel",        new WrapResetModel);
        SymbolTable::ScrDefConst(L"SetPoi",            new WrapSetPoi);
        SymbolTable::ScrDefConst(L"ClearPoi",          new WrapClearPoi);
        SymbolTable::ScrDefConst(L"Include",           new WrapInclude);
        SymbolTable::ScrDefConst(L"Pause",             new WrapPause);
        SymbolTable::ScrDefConst(L"ModelDoEdit",       new WrapModelDoEdit);
        SymbolTable::ScrDefConst(L"SetBrushShape",     new WrapSetBrushShape);
        SymbolTable::ScrDefConst(L"SetBrushRadius",    new WrapSetBrushRadius);
        SymbolTable::ScrDefConst(L"SetBrushIntensity", new WrapSetBrushIntensity);
        SymbolTable::ScrDefConst(L"SetBrushMode",      new WrapSetBrushMode);

		DEF_ULONG_CONST(Strategy::Id::defect);
		DEF_ULONG_CONST(Strategy::Id::cooperate);
		DEF_ULONG_CONST(Strategy::Id::tit4tat);

		DEF_ULONG_CONST(tShape::Rect);
		DEF_ULONG_CONST(tShape::Circle);
		DEF_ULONG_CONST(tShape::GridModel);

		DEF_ULONG_CONST(tManipulator::set);
		DEF_ULONG_CONST(tManipulator::max);
		DEF_ULONG_CONST(tManipulator::min);
		DEF_ULONG_CONST(tManipulator::add);
		DEF_ULONG_CONST(tManipulator::subtract);
		DEF_ULONG_CONST(tManipulator::mean);

		DEF_ULONG_CONST(tBrushMode::move);
		DEF_ULONG_CONST(tBrushMode::randomStrat);
		DEF_ULONG_CONST(tBrushMode::cooperate);
		DEF_ULONG_CONST(tBrushMode::defect);
		DEF_ULONG_CONST(tBrushMode::tit4tat);
		DEF_ULONG_CONST(tBrushMode::noAnimals);
		DEF_ULONG_CONST(tBrushMode::mutRate);
		DEF_ULONG_CONST(tBrushMode::fertility);
		DEF_ULONG_CONST(tBrushMode::food);
		DEF_ULONG_CONST(tBrushMode::fertilizer);
	}
}
