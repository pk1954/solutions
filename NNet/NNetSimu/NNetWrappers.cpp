// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#include "stdafx.h"
#include "symtab.h"
#include "Resource.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "NNetWrapperHelpers.h"
#include "DrawContext.h"
#include "win32_NNetWindow.h"
#include "NNetModelWriterInterface.h"

static NNetModelWriterInterface * m_pModelWriterInterface;
static NNetWindow               * m_pNNetWindow;

class WrapResetTimer: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->ResetTimer( );
    }
};

class WrapResetModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->ResetModel( );
    }
};

class WrapConnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const idSrc { ScrReadShapeId( script ) };
        ShapeId const idDst { ScrReadShapeId( script ) };
        m_pModelWriterInterface->Connect( idSrc, idDst );
    }
};

class WrapDeleteShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pModelWriterInterface->DeleteShape( id );
    }
};

class WrapDisconnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pModelWriterInterface->Disconnect( id );
    }
};

class WrapSetPulseRate: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id     { ScrReadShapeId( script ) };
        float   const fValue { CastToFloat( script.ScrReadFloat( ) ) };
        m_pModelWriterInterface->SetPulseRate( id, fHertz{ fValue } );
    }
};

class WrapSetParameter: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        tParameter const param  { static_cast<tParameter>( script.ScrReadUlong( ) ) };
        float      const fValue { CastToFloat( script.ScrReadFloat( ) ) };
        m_pModelWriterInterface->SetParameter( param, fValue );
    }
};

class WrapMoveShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const id      { ScrReadShapeId( script ) };
        MicroMeterPoint const umDelta { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->MoveShape( id, umDelta );
    }
};

class WrapAddOutgoing2Knot: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->AddOutgoing2Knot( idShape, umPos );
    }
};

class WrapAddIncoming2Knot: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->AddIncoming2Knot( idShape, umPos );
    }
};

class WrapAddOutgoing2Pipe: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->AddOutgoing2Pipe( idShape, umPos );
    }
};

class WrapAddIncoming2Pipe: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->AddIncoming2Pipe( idShape, umPos );
    }
};

class WrapSetPixelOffset: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        fPixelPoint const fPixelOffset { ScrReadfPixelPoint( script ) };
        m_pNNetWindow->GetDrawContext().SetPixelOffset( fPixelOffset );
    }
};

class WrapSetPixelSize: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeter const umPixelSize { ScrReadMicroMeter( script ) };
        m_pNNetWindow->GetDrawContext().SetPixelSize( umPixelSize );
    }
};

class WrapBreak : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int x = 42;
    }
};

void DefineNNetWrappers
( 
    NNetModelWriterInterface * const pModel,
    NNetWindow               * const pNNetWindow
)
{
    m_pModelWriterInterface = pModel;
    m_pNNetWindow = pNNetWindow;

    DEF_FUNC( ResetTimer );
    DEF_FUNC( ResetModel );
    DEF_FUNC( Connect );
    DEF_FUNC( DeleteShape );
    DEF_FUNC( Disconnect );
    DEF_FUNC( SetPulseRate );
    DEF_FUNC( SetParameter );
    DEF_FUNC( MoveShape );
    DEF_FUNC( AddOutgoing2Knot );
    DEF_FUNC( AddIncoming2Knot );
    DEF_FUNC( AddOutgoing2Pipe );
    DEF_FUNC( AddIncoming2Pipe );
    DEF_FUNC( SetPixelOffset );
    DEF_FUNC( SetPixelSize );
    DEF_FUNC( Break );

    Apply2GlobalParameters
    (  
        [&]( tParameter const & param ) 
        {
            SymbolTable::ScrDefConst( GetParameterName( param ), static_cast<unsigned long>( param ) );
        }
    );

    SymbolTable::ScrDefConst( L"ANALYZE_LOOPS",       static_cast<long>(IDM_ANALYZE_LOOPS      ) );
    SymbolTable::ScrDefConst( L"ANALYZE_ANOMALIES",   static_cast<long>(IDM_ANALYZE_ANOMALIES  ) );
    SymbolTable::ScrDefConst( L"DELETE_SELECTION",    static_cast<long>(IDM_DELETE_SELECTION   ) );
    SymbolTable::ScrDefConst( L"CLEAR_BEEPERS",       static_cast<long>(IDM_CLEAR_BEEPERS      ) );
    SymbolTable::ScrDefConst( L"SELECT_ALL_BEEPERS",  static_cast<long>(IDM_SELECT_ALL_BEEPERS ) );
    SymbolTable::ScrDefConst( L"MARK_SELECTION",      static_cast<long>(IDM_MARK_SELECTION     ) );
    SymbolTable::ScrDefConst( L"COPY_SELECTION",      static_cast<long>(IDM_COPY_SELECTION     ) );
    SymbolTable::ScrDefConst( L"INSERT_NEURON",       static_cast<long>(IDD_INSERT_NEURON      ) );
    SymbolTable::ScrDefConst( L"NEW_NEURON",          static_cast<long>(IDD_NEW_NEURON         ) );
    SymbolTable::ScrDefConst( L"NEW_INPUT_NEURON",    static_cast<long>(IDD_NEW_INPUT_NEURON   ) );
    SymbolTable::ScrDefConst( L"APPEND_NEURON",       static_cast<long>(IDD_APPEND_NEURON      ) );
    SymbolTable::ScrDefConst( L"APPEND_INPUT_NEURON", static_cast<long>(IDD_APPEND_INPUT_NEURON) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2KNOT",   static_cast<long>(IDD_ADD_OUTGOING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2KNOT",   static_cast<long>(IDD_ADD_INCOMING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2PIPE",   static_cast<long>(IDD_ADD_OUTGOING2PIPE  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2PIPE",   static_cast<long>(IDD_ADD_INCOMING2PIPE  ) );
}
