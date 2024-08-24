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
import :GridPOI;

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

		SymbolTable::ScrDefConst(L"Strategy::Id::defect",    static_cast<unsigned long>(Strategy::Id::defect   ));
		SymbolTable::ScrDefConst(L"Strategy::Id::cooperate", static_cast<unsigned long>(Strategy::Id::cooperate));
		SymbolTable::ScrDefConst(L"Strategy::Id::tit4tat",   static_cast<unsigned long>(Strategy::Id::tit4tat  ));

        SymbolTable::ScrDefConst(L"tShape::Rect",            static_cast<unsigned long>(tShape::Rect           ));
		SymbolTable::ScrDefConst(L"tShape::Circle",          static_cast<unsigned long>(tShape::Circle         ));
		SymbolTable::ScrDefConst(L"tShape::GridModel",       static_cast<unsigned long>(tShape::GridModel      ));

		SymbolTable::ScrDefConst(L"tManipulator::set",       static_cast<unsigned long>(tManipulator::set      ));
		SymbolTable::ScrDefConst(L"tManipulator::max",       static_cast<unsigned long>(tManipulator::max      ));
		SymbolTable::ScrDefConst(L"tManipulator::min",       static_cast<unsigned long>(tManipulator::min      ));
		SymbolTable::ScrDefConst(L"tManipulator::add",       static_cast<unsigned long>(tManipulator::add      ));
		SymbolTable::ScrDefConst(L"tManipulator::subtract",  static_cast<unsigned long>(tManipulator::subtract ));
		SymbolTable::ScrDefConst(L"tManipulator::mean",      static_cast<unsigned long>(tManipulator::mean     ));

        SymbolTable::ScrDefConst(L"tBrushMode::move",        static_cast<unsigned long>(tBrushMode::move       ));
		SymbolTable::ScrDefConst(L"tBrushMode::randomStrat", static_cast<unsigned long>(tBrushMode::randomStrat));
		SymbolTable::ScrDefConst(L"tBrushMode::cooperate",   static_cast<unsigned long>(tBrushMode::cooperate  ));
		SymbolTable::ScrDefConst(L"tBrushMode::defect",      static_cast<unsigned long>(tBrushMode::defect     ));
		SymbolTable::ScrDefConst(L"tBrushMode::tit4tat",     static_cast<unsigned long>(tBrushMode::tit4tat    ));
		SymbolTable::ScrDefConst(L"tBrushMode::noAnimals",   static_cast<unsigned long>(tBrushMode::noAnimals  ));
		SymbolTable::ScrDefConst(L"tBrushMode::mutRate",     static_cast<unsigned long>(tBrushMode::mutRate    ));
		SymbolTable::ScrDefConst(L"tBrushMode::fertility",   static_cast<unsigned long>(tBrushMode::fertility  ));
		SymbolTable::ScrDefConst(L"tBrushMode::food",        static_cast<unsigned long>(tBrushMode::food       ));
		SymbolTable::ScrDefConst(L"tBrushMode::fertilizer",  static_cast<unsigned long>(tBrushMode::fertilizer ));
	}
}
