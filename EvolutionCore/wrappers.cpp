// wrappers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "EvolutionTypes.h"
#include "EditorState.h"
#include "gridCircle.h"
#include "wrapperHelpers.h"
#include "wrappers.h"

//lint -e534                 // ignoring return value of function
//lint -esym( 715, script )  // not referenced

static EvolutionCore * m_pCore;
static ModelData     * m_pModelWork;

class WrapSetConfigValue : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        unsigned long const ulConfigId = script.ScrReadUlong( );
        Config::tId   const id         = static_cast<Config::tId>( ulConfigId );
        long          const lValue     = script.ScrReadLong();

        Config::SetConfigValue( id, lValue );
    }
};

class WrapSetPoi : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        GridPoint gp = ScrReadGridPoint( script );
        m_pCore->SetPoi( m_pModelWork, gp );
    }
};

class WrapClearPoi : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCore->ClearPoi( );
    }
};

class WrapResetModel : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCore->ResetModel( m_pModelWork );
    }
};

class WrapPause : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    { }
};

class WrapModelDoEdit : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        GridPoint gp = ScrReadGridPoint( script );
        m_pModelWork->ModelDoEdit( gp );
    }
};


class WrapSetBrushShape : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int    const iShape = script.ScrReadInt( );
        tShape const shape  = static_cast<tShape>( iShape );
        m_pModelWork->SetBrushShape( shape );
    }
};

class WrapSetBrushSize : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        short sSize = script.ScrReadShort( );
        m_pModelWork->SetBrushSize( sSize );
    }
};

class WrapSetBrushIntensity : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        short sIntensity = script.ScrReadShort( );
        m_pModelWork->SetBrushIntensity( sIntensity );
    }
};

class WrapSetBrushStrategy : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int        const iMode = script.ScrReadInt( );
        tBrushMode const mode  = static_cast<tBrushMode>( iMode );
        m_pModelWork->SetBrushStrategy( mode );
    }
};

void DefineModelWrapperFunctions( ModelData * pModel )
{
    m_pModelWork = pModel;

    DEF_FUNC( ModelDoEdit );
    DEF_FUNC( SetBrushShape );
    DEF_FUNC( SetBrushSize );
    DEF_FUNC( SetBrushIntensity );
    DEF_FUNC( SetBrushStrategy );
}

void DefineCoreWrapperFunctions( EvolutionCore * pCore )
{
    m_pCore = pCore;

    DEF_FUNC( ResetModel );
    DEF_FUNC( SetConfigValue );
    DEF_FUNC( SetPoi );
    DEF_FUNC( ClearPoi );
    DEF_FUNC( Include );
    DEF_FUNC( Pause );

    DEF_ULONG_CONST( tStrategyId::defectAlways );
    DEF_ULONG_CONST( tStrategyId::cooperateAlways );
    DEF_ULONG_CONST( tStrategyId::tit4tat );

    DEF_ULONG_CONST( tShape::Rect );
    DEF_ULONG_CONST( tShape::Circle );

    DEF_ULONG_CONST( tBrushMode::move );
    DEF_ULONG_CONST( tBrushMode::randomStrategy );
    DEF_ULONG_CONST( tBrushMode::cooperate );
    DEF_ULONG_CONST( tBrushMode::defect );
    DEF_ULONG_CONST( tBrushMode::tit4tat );
    DEF_ULONG_CONST( tBrushMode::noAnimals );
    DEF_ULONG_CONST( tBrushMode::mutRate );
    DEF_ULONG_CONST( tBrushMode::fertility );
    DEF_ULONG_CONST( tBrushMode::food );
    DEF_ULONG_CONST( tBrushMode::fertilizer );
}
