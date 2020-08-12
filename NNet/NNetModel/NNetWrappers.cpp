// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetSimu

#include "stdafx.h"
#include "symtab.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "NNetWrapperHelpers.h"
#include "DrawContext.h"
#include "NNetModelWriterInterface.h"

static NNetModelWriterInterface * m_pModelWriterInterface;

class WrapResetModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->ResetModel( );
    }
};

class WrapReadModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        wstring const wstrModelPath { script.ScrReadString( ) };
        m_pModelWriterInterface->ReadModel( false, wstrModelPath );
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

class WrapNewNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->NewNeuron( umPos );
    }
};

class WrapNewInputNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->NewInputNeuron( umPos );
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

void DefineNNetWrappers( NNetModelWriterInterface * const pModel )
{
    m_pModelWriterInterface = pModel;

    DEF_FUNC( ResetModel );
    DEF_FUNC( ReadModel );
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
    DEF_FUNC( NewNeuron );
    DEF_FUNC( NewInputNeuron );
    DEF_FUNC( Break );

    Apply2GlobalParameters
    (  
        [&]( tParameter const & param ) 
        {
            SymbolTable::ScrDefConst( GetParameterName( param ), static_cast<unsigned long>( param ) );
        }
    );
}
