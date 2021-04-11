// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetSimu

#include "stdafx.h"
#include "symtab.h"
#include "ShapeIdList.h"
#include "SoundInterface.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "DrawContext.h"
#include "NNetError.h"
#include "NNetWrapperHelpers.h"
#include "NNetModelCommands.h"

static NNetModelCommands * m_pCommands;

class WrapAnalyzeAnomalies: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->AnalyzeAnomalies();
    }
};

class WrapAnalyzeLoops: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->AnalyzeLoops();
    }
};

class WrapResetModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->ResetModel();
    }
};

class WrapCreateInitialShapes: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->CreateInitialShapes();
    }
};

class WrapCreateConnector: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->CreateConnector(ScrReadShapeIdList(script));
    }
};

class WrapConnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const idSrc { ScrReadShapeId( script ) };
        ShapeId const idDst { ScrReadShapeId( script ) };
        m_pCommands->Connect( idSrc, idDst );
    }
};

class WrapAppendInputNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pCommands->AppendInputNeuron( id );
    }
};

class WrapAppendNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pCommands->AppendNeuron( id );
    }
};

class WrapClearBeepers: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->ClearBeepers();
    }
};

class WrapCopySelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->CopySelection();
    }
};

class WrapSelectAll: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        bool const bOn { script.ScrReadInt() != 0 };
        m_pCommands->SelectAll(bOn);
    }
};

class WrapSelectSubtree: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id  { ScrReadShapeId(script) };
        bool    const bOn { script.ScrReadInt() != 0 };
        m_pCommands->SelectSubtree(id, bOn);
    }
};

class WrapSelectShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        tBoolOp const op { ScrReadBoolOp( script ) };
        m_pCommands->SelectShape( id, op );
    }
};

class WrapSetTriggerSound: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        SoundDescr desc 
        {
            script.ScrReadInt() != 0,
            Hertz(script.ScrReadUlong()),
            MilliSecs(script.ScrReadUlong())
        };
        m_pCommands->SetTriggerSound( id, desc );
    }
};

class WrapToggleStopOnTrigger: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pCommands->ToggleStopOnTrigger( id );
    }
};

class WrapSelectAllBeepers: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->SelectAllBeepers();
    }
};

class WrapDeleteSelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->DeleteSelection();
    }
};

class WrapDeleteShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pCommands->DeleteShape( id );
    }
};

class WrapDisconnect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id { ScrReadShapeId( script ) };
        m_pCommands->Disconnect( id );
    }
};

class WrapSetPulseRate: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId const id     { ScrReadShapeId( script ) };
        float   const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_pCommands->SetPulseRate( id, fHertz{ fValue } );
    }
};

class WrapSetConnectionNeurons: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        unique_ptr<ShapeIdList> upShapeIds  { ScrReadShapeIdList(script) };
        MicroMeterPointVector   umPntVector { ScrReadMicroMeterPointVector(script) };
        m_pCommands->SetConnectionNeurons( umPntVector, move(upShapeIds) );
    }
};

class WrapSetParameter: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ParamType::Value const param  { static_cast<ParamType::Value>( script.ScrReadUlong() ) };
        float            const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_pCommands->SetParameter( param, fValue );
    }
};

class WrapMoveShape: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const id      { ScrReadShapeId( script ) };
        MicroMeterPoint const umDelta { ScrReadMicroMeterPoint( script ) };
        m_pCommands->MoveShape( id, umDelta );
    }
};

class WrapSelectShapesInRect: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPntStart { ScrReadMicroMeterPoint( script ) };
        MicroMeterPoint const umPntEnd   { ScrReadMicroMeterPoint( script ) };
        bool            const bOn        { script.ScrReadUint() != 0 };
        m_pCommands->SelectShapesInRect( MicroMeterRect(umPntStart, umPntEnd), bOn );
    }
};

class WrapMoveSelection: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pCommands->MoveSelection( umPos );
    }
};

class WrapInsertNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const id    { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pCommands->InsertNeuron( id, umPos );
    }
};

class WrapAddOutgoing2Knot: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pCommands->AddOutgoing2Knot( idShape, umPos );
    }
};

class WrapAddIncoming2Knot: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pCommands->AddIncoming2Knot( idShape, umPos );
    }
};

class WrapAddOutgoing2Pipe: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pCommands->AddOutgoing2Pipe( idShape, umPos );
    }
};

class WrapAddIncoming2Pipe: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ShapeId         const idShape { ScrReadShapeId( script ) };
        MicroMeterPoint const umPos   { ScrReadMicroMeterPoint( script ) };
        m_pCommands->AddIncoming2Pipe( idShape, umPos );
    }
};

class WrapNewNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pCommands->NewNeuron( umPos );
    }
};

class WrapNewInputNeuron: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeterPoint const umPos { ScrReadMicroMeterPoint( script ) };
        m_pCommands->NewInputNeuron( umPos );
    }
};

class WrapUndoCommand: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->UndoCommand();
    }
};

class WrapRedoCommand: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCommands->RedoCommand();
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

void DefineNNetWrappers( NNetModelCommands * const pCommands )
{
    m_pCommands = pCommands;

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
    DEF_FUNC(CreateConnector);
    DEF_FUNC(CreateInitialShapes);
    DEF_FUNC(DeleteSelection);    
    DEF_FUNC(DeleteShape);        
    DEF_FUNC(Disconnect);         
    DEF_FUNC(InsertNeuron);       
    DEF_FUNC(MoveSelection);      
    DEF_FUNC(MoveShape);          
    DEF_FUNC(NewInputNeuron);     
    DEF_FUNC(NewNeuron);          
    DEF_FUNC(ResetModel);         
    DEF_FUNC(SelectAll);          
    DEF_FUNC(SelectAllBeepers);   
    DEF_FUNC(SelectShape);        
    DEF_FUNC(SelectShapesInRect); 
    DEF_FUNC(SelectSubtree);      
    DEF_FUNC(SetParameter);    
    DEF_FUNC(SetPulseRate); 
    DEF_FUNC(SetTriggerSound);
    DEF_FUNC(SetConnectionNeurons);
    DEF_FUNC(ToggleStopOnTrigger);
    DEF_FUNC(UndoCommand);
    DEF_FUNC(RedoCommand);
    DEF_FUNC(Break );

    ParamType::Apply2GlobalParameters
    (  
        [&]( ParamType::Value const & param ) 
        {
            SymbolTable::ScrDefConst(ParamType::GetName(param), static_cast<unsigned long>(param));
        }
    );
}
