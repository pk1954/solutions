// win32_wrappers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "symtab.h"
#include "Resource.h"
#include "EvoGenerationCmd.h"
#include "EvolutionCoreWrapperHelpers.h"
#include "win32_workThreadInterface.h"
#include "win32_wrappers.h"
#include "win32_util.h"

static WorkThreadInterface * m_pWorkThreadInterface;

class WrapPostPrevGeneration : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostPrevGeneration( );
    }
};

class WrapPostBenchmark : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
		int iNrOfGenerations = script.ScrReadInt( );
        m_pWorkThreadInterface->PostBenchmark( iNrOfGenerations );
    }
};

class WrapPostDoEdit : public Script_Functor
{
public:
	virtual void operator() ( Script & script ) const
	{
		GridPoint gp = ScrReadGridPoint( script );
		m_pWorkThreadInterface->PostDoEdit( gp );
	}
};

class WrapPostSetPOI : public Script_Functor
{
public:
	virtual void operator() ( Script & script ) const
	{
		GridPoint gp = ScrReadGridPoint( script );
		m_pWorkThreadInterface->PostSetPOI( gp );
	}
};

class WrapPostSetSimulationMode : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ULONG   const ulOp = script.ScrReadUlong( );
        tBoolOp const op   = static_cast<tBoolOp>( ulOp );
        m_pWorkThreadInterface->PostSetSimulationMode( op );
    }
};

class WrapPostSetBrushMode : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ULONG      const ulCmd = script.ScrReadUlong( );
        tBrushMode const mode  = static_cast<tBrushMode>( ulCmd );
        m_pWorkThreadInterface->PostSetBrushMode( mode );
    }
};

class WrapPostSetBrushShape : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ULONG  const ulShape = script.ScrReadUlong( );
        tShape const shape   = static_cast<tShape>( ulShape );
        m_pWorkThreadInterface->PostSetBrushShape( shape );
    }
};

class WrapPostSetBrushIntensity : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
		PERCENT const intensity { script.ScrReadShort( ) };
        m_pWorkThreadInterface->PostSetBrushIntensity( intensity );
    }
};

class WrapPostSetBrushRadius : public Script_Functor
{
public:
	virtual void operator() ( Script & script ) const
	{
		GRID_COORD const size = ScrReadGridCoord( script );
		m_pWorkThreadInterface->PostSetBrushRadius( size );
	}
};

class WrapPostSetBrushManipulator : public Script_Functor
{
public:
	virtual void operator() ( Script & script ) const
	{
		USHORT       const usValue     = script.ScrReadUshort(  );
		tManipulator const manipulator = static_cast<tManipulator>( usValue );
		m_pWorkThreadInterface->PostSetBrushManipulator( manipulator );
	}
};

class WrapPostGenerationStep : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostGenerationStep( );
    }
};

class WrapPostRunGenerations : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostGenerationStep( );
    }
};

class WrapPostProcessScript : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        wstring const wstrScript = script.ScrReadString( );
        m_pWorkThreadInterface->PostProcessScript( wstrScript );
    }
};

class WrapPostRefresh : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostRefresh( false );
    }
};

class WrapPostReset : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        INT const iValue = script.ScrReadInt( );
        m_pWorkThreadInterface->PostReset( iValue != 0 );
    }
};

class WrapBreak : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
    }
};

void DefineWin32WrapperFunctions( WorkThreadInterface * const pWorkThreadInterface )
{
    m_pWorkThreadInterface = pWorkThreadInterface;

    DEF_FUNC( PostPrevGeneration ); 

	DEF_FUNC( PostBenchmark );
	DEF_FUNC( PostDoEdit );
	DEF_FUNC( PostSetPOI );
	DEF_FUNC( PostSetSimulationMode );
    DEF_FUNC( PostSetBrushMode );
    DEF_FUNC( PostSetBrushShape );
    DEF_FUNC( PostSetBrushIntensity );
    DEF_FUNC( PostSetBrushRadius );
	DEF_FUNC( PostSetBrushManipulator );
    DEF_FUNC( PostRefresh );
    DEF_FUNC( PostReset );
    DEF_FUNC( PostGenerationStep );
    DEF_FUNC( PostRunGenerations );
    DEF_FUNC( PostProcessScript );
    DEF_FUNC( Break );

    DEF_ULONG_CONST( IDM_GENERATION );
    DEF_ULONG_CONST( IDM_BACKWARDS );
    DEF_ULONG_CONST( IDM_RUN );
    DEF_ULONG_CONST( IDM_STOP );

    DEF_ULONG_CONST( tEvoCmd::setSimulationMode );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushMode );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushShape );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushRadius );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushIntensity );
    DEF_ULONG_CONST( tEvoCmd::editDoEdit );
    DEF_ULONG_CONST( tEvoCmd::editSetPOI );
}
