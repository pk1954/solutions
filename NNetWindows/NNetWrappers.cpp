// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#include "stdafx.h"
#include "symtab.h"
#include "Resource.h"
#include "win32_util.h"
#include "NNetWrapperHelpers.h"
#include "win32_NNetWorkThreadInterface.h"

static NNetWorkThreadInterface * m_pWorkThreadInterface;

class WrapPostResetTimer: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostResetTimer( );
    }
};

class WrapPostResetModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThreadInterface->PostResetModel( );
    }
};

class WrapPostConnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const idSrc { ScrReadShapeId( script ) };
        ShapeId const idDst { ScrReadShapeId( script ) };
        m_pWorkThreadInterface->PostConnect( idSrc, idDst );
    }
};

class WrapPostRemoveShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pWorkThreadInterface->PostRemoveShape( id );
    }
};

class WrapPostDisconnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pWorkThreadInterface->PostDisconnect( id );
    }
};

class WrapPostSetPulseRate: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id     { ScrReadShapeId( script ) };
        float   const fValue { CastToFloat( script.ScrReadFloat( ) ) };
        m_pWorkThreadInterface->PostSetPulseRate( id, fValue );
    }
};

class WrapPostSetParameter: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        tParameter const param  { static_cast<tParameter>( script.ScrReadUlong( ) ) };
        float      const fValue { CastToFloat( script.ScrReadFloat( ) ) };
        m_pWorkThreadInterface->PostSetParameter( param, fValue );
    }
};

class WrapPostMoveShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const id      { ScrReadShapeId( script ) };
        MicroMeterPoint const umDelta { ScrReadMicroMeterPoint( script ) };
        m_pWorkThreadInterface->PostMoveShape( id, umDelta );
    }
};

class WrapPostSlowMotionChanged: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        unsigned int const uiRatio { script.ScrReadUint( ) };
        m_pWorkThreadInterface->PostSlowMotionChanged( uiRatio );
    }
};

class WrapPostActionCommand: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int             const idMsg   { script.ScrReadLong( ) };
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pWorkThreadInterface->PostActionCommand( idMsg, idShape, umPos );
    }
};

class WrapBreak : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int x = 75;
    }
};

void DefineNNetWrappers( NNetWorkThreadInterface * const pWorkThreadInterface )
{
    m_pWorkThreadInterface = pWorkThreadInterface;

    DEF_FUNC( PostResetTimer );
    DEF_FUNC( PostResetModel );
    DEF_FUNC( PostConnect );
    DEF_FUNC( PostRemoveShape );
    DEF_FUNC( PostDisconnect );
    DEF_FUNC( PostSetPulseRate );
    DEF_FUNC( PostSetParameter );
    DEF_FUNC( PostMoveShape );
    DEF_FUNC( PostSlowMotionChanged );
    DEF_FUNC( PostActionCommand );
    DEF_FUNC( Break );

    Apply2GlobalParameters
    (  
        [&]( tParameter const & param ) 
        {
            SymbolTable::ScrDefConst( GetParameterName( param ), static_cast<unsigned long>( param ) );
        }
    );

    SymbolTable::ScrDefConst( L"INSERT_NEURON",       static_cast<long>(IDD_INSERT_NEURON      ) );
    SymbolTable::ScrDefConst( L"NEW_NEURON",          static_cast<long>(IDD_NEW_NEURON         ) );
    SymbolTable::ScrDefConst( L"NEW_INPUT_NEURON",    static_cast<long>(IDD_NEW_INPUT_NEURON   ) );
    SymbolTable::ScrDefConst( L"APPEND_NEURON",       static_cast<long>(IDD_APPEND_NEURON      ) );
    SymbolTable::ScrDefConst( L"APPEND_INPUT_NEURON", static_cast<long>(IDD_APPEND_INPUT_NEURON) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2KNOT",   static_cast<long>(IDD_ADD_OUTGOING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2KNOT",   static_cast<long>(IDD_ADD_INCOMING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2PIPE",   static_cast<long>(IDD_ADD_OUTGOING2PIPE  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2PIPE",   static_cast<long>(IDD_ADD_INCOMING2PIPE  ) );

//    DEF_ULONG_CONST( EvoGenerationCmd::Id::editSetBrushMode );
}
