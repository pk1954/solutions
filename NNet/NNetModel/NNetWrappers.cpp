// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetSimu

#include "stdafx.h"
#include "symtab.h"
#include "SoundInterface.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "DrawContext.h"
#include "NNetError.h"
#include "NNetWrapperHelpers.h"
#include "NNetModelWriterInterface.h"

static NNetModelWriterInterface * m_pModelWriterInterface;

class WrapAnalyzeAnomalies: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->AnalyzeAnomalies( );
    }
};

class WrapAnalyzeLoops: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->AnalyzeLoops( );
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

class WrapAppendInputNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pModelWriterInterface->AppendInputNeuron( id );
    }
};

class WrapAppendNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pModelWriterInterface->AppendNeuron( id );
    }
};

class WrapClearBeepers: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->ClearBeepers( );
    }
};

class WrapCopySelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->CopySelection( );
    }
};

class WrapSelectAll: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        tBoolOp const op { ScrReadBoolOp( script ) };
        m_pModelWriterInterface->SelectAll( op );
    }
};

class WrapSelectSubtree: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        tBoolOp const op { ScrReadBoolOp( script ) };
        m_pModelWriterInterface->SelectSubtree( id, op );
    }
};

class WrapSelectShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        tBoolOp const op { ScrReadBoolOp( script ) };
        m_pModelWriterInterface->SelectShape( id, op );
    }
};

class WrapSetTriggerSound: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        NNetErrorHandler::CheckShapeId( script, m_pModelWriterInterface->GetModel(), id );
        SoundDescr desc 
        {
            script.ScrReadInt() != 0,
            Hertz(script.ScrReadUlong()),
            MilliSecs(script.ScrReadUlong())
        };
        m_pModelWriterInterface->SetTriggerSound( id, desc );
    }
};

class WrapToggleStopOnTrigger: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pModelWriterInterface->ToggleStopOnTrigger( id );
    }
};

class WrapSelectAllBeepers: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->SelectAllBeepers( );
    }
};

class WrapDeleteSelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->DeleteSelection( );
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

class WrapSelectShapesInRect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPntStart { ScrReadMicroMeterPoint( script ) };
        MicroMeterPoint const umPntEnd   { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->SelectShapesInRect( MicroMeterRect( umPntStart, umPntEnd ) );
    }
};

class WrapMarkSelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        tBoolOp const op { ScrReadBoolOp( script ) };
        m_pModelWriterInterface->MarkSelection( op );
    }
};

class WrapMoveSelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->MoveSelection( umPos );
    }
};

class WrapInsertNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const id    { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pModelWriterInterface->InsertNeuron( id, umPos );
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

class WrapUndoCommand: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->UndoCommand( );
    }
};

class WrapRedoCommand: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWriterInterface->RedoCommand( );
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

    DEF_FUNC(AnalyzeAnomalies);   
    DEF_FUNC(AnalyzeLoops);       
    DEF_FUNC(AppendInputNeuron);  
    DEF_FUNC(AppendNeuron);       
    DEF_FUNC(AddIncoming2Knot);   
    DEF_FUNC(AddIncoming2Pipe);   
    DEF_FUNC(AddOutgoing2Knot);   
    DEF_FUNC(AddOutgoing2Pipe);   
    DEF_FUNC(ClearBeepers);       
    DEF_FUNC(Connect);            
    DEF_FUNC(CopySelection);      
    DEF_FUNC(DeleteSelection);    
    DEF_FUNC(DeleteShape);        
    DEF_FUNC(Disconnect);         
    DEF_FUNC(InsertNeuron);       
    DEF_FUNC(MarkSelection);      
    DEF_FUNC(MoveSelection);      
    DEF_FUNC(MoveShape);          
    DEF_FUNC(NewInputNeuron);     
    DEF_FUNC(NewNeuron);          
    DEF_FUNC(ReadModel);          
    DEF_FUNC(ResetModel);         
    DEF_FUNC(SelectAll);          
    DEF_FUNC(SelectAllBeepers);   
    DEF_FUNC(SelectShape);        
    DEF_FUNC(SelectShapesInRect); 
    DEF_FUNC(SelectSubtree);      
    DEF_FUNC(SetParameter);    
    DEF_FUNC(SetPulseRate); 
    DEF_FUNC(SetTriggerSound);
    DEF_FUNC(ToggleStopOnTrigger);
    DEF_FUNC(UndoCommand);
    DEF_FUNC(RedoCommand);
    DEF_FUNC(Break );

    Apply2GlobalParameters
    (  
        [&]( tParameter const & param ) 
        {
            SymbolTable::ScrDefConst( GetParameterName( param ), static_cast<unsigned long>( param ) );
        }
    );
}
