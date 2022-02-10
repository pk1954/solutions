// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ERRHNDL.H"
#include "SYMTAB.H"
#include "NobType.h"
#include "NobIdList.h"
#include "SoundInterface.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "DrawContext.h"
#include "win32_importTermination.h"
#include "NNetModelImporter.h"
#include "NNetModelReaderInterface.h"
#include "NNetWrapperHelpers.h"
#include "NNetModelCommands.h"

static NNetModelReaderInterface * m_pNMRI;
static NNetModelCommands        * m_pCommands;
static NNetModelImporter        * m_pModelImporter;

class WrapConnect: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const idSrc { ScrReadNobId(script) };
        NobId const idDst { ScrReadNobId(script) };
        if (m_pNMRI->CanConnectTo(idSrc, idDst))
        {
            m_pCommands->Connect(idSrc, idDst);
        }
        else
        {
            script.SetExpectedToken(L"");
            wstring wstrMsg
            {
                L"Invalid: Connect " + 
                m_pNMRI->GetTypeName(idSrc) + 
                L" to " + 
                m_pNMRI->GetTypeName(idDst)
            };
            throw ScriptErrorHandler::ScriptException(999, wstrMsg);
        }
    }
};

class WrapMakeIoConnector: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->MakeIoConnector();
    }
};

class WrapAddModel: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pModelImporter->Import
        (
            script.ScrReadString(), 
            NNetImportTermination::CreateNew(IDM_ADD_IMPORTED_MODEL)
        );
    }
};

class WrapAddSignal: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterCircle const umCircle { ScrReadMicroMeterCircle(script) };
        TrackNr          const trackNr  { ScrReadTrackNr(script) };
        m_pCommands->AddSignal(umCircle, trackNr);
    }
};

class WrapAnalyzeAnomalies: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->AnalyzeAnomalies();
    }
};

class WrapAnalyzeLoops: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->AnalyzeLoops();
    }
};

class WrapResetModel: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->ResetModel();
    }
};

class WrapCreateInitialNobs: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->CreateInitialNobs();
    }
};

class WrapAppendInputNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->AppendInputNeuron(id);
    }
};

class WrapAppendOutputNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->AppendOutputNeuron(id);
    }
};

class WrapClearBeepers: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->ClearBeepers();
    }
};

class WrapCopySelection: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->CopySelection();
    }
};

class WrapSelectAll: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        bool const bOn { script.ScrReadInt() != 0 };
        m_pCommands->SelectAll(bOn);
    }
};

class WrapSelectSubtree: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id  { ScrReadNobId(script) };
        bool  const bOn { script.ScrReadInt() != 0 };
        m_pCommands->SelectSubtree(id, bOn);
    }
};

class WrapSelectNob: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId   const id { ScrReadNobId(script) };
        tBoolOp const op { ScrReadBoolOp(script) };
        m_pCommands->SelectNob(id, op);
    }
};

class WrapSetTriggerSound: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        SoundDescr desc 
        {
            script.ScrReadInt() != 0,
            Hertz(script.ScrReadUlong()),
            MilliSecs(script.ScrReadUlong())
        };
        m_pCommands->SetTriggerSound(id, desc);
    }
};

class WrapToggleStopOnTrigger: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->ToggleStopOnTrigger(id);
    }
};

class WrapSelectAllBeepers: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->SelectAllBeepers();
    }
};

class WrapDeleteSelection: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->DeleteSelection();
    }
};

class WrapDeleteNob: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->DeleteNob(id);
    }
};

class WrapDiscBaseKnot: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->DiscBaseKnot(id);
    }
};

class WrapDiscIoConnector: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->DiscIoConnector(id);
    }
};

class WrapSplitNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->SplitNeuron(id);
    }
};

class WrapSetPulseRate: public ScriptFunctor  // Legacy
{
public:
    void operator() (Script & script) const final
    {
        ScrReadNobId(script);
        Cast2Float(script.ScrReadFloat());
    }
};

class WrapSetParameter: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        ParamType::Value const param  { static_cast<ParamType::Value>(script.ScrReadUlong()) };
        float            const fValue { Cast2Float(script.ScrReadFloat()) };
        m_pCommands->SetParameter(param, fValue);
    }
};

class WrapMoveNob: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const id      { ScrReadNobId(script) };
        MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
        m_pCommands->MoveNob(id, umDelta);
    }
};

class WrapMoveSensor: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        SignalId      const id      { ScrReadSignalId(script) };
        MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
        m_pCommands->MoveSensor(id, umDelta);
    }
};

class WrapSelectNobsInRect: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterPnt const umPntStart { ScrReadMicroMeterPnt(script) };
        MicroMeterPnt const umPntEnd   { ScrReadMicroMeterPnt(script) };
        m_pCommands->SelectNobsInRect(MicroMeterRect(umPntStart, umPntEnd));
    }
};

class WrapMoveSelection: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->MoveSelection(umPos);
    }
};

class WrapInsertNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const id    { ScrReadNobId(script) };
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->InsertNeuron(id, umPos);
    }
};

class WrapAddOutgoing2Knot: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const idNob { ScrReadNobId(script) };
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->AddOutgoing2Knot(idNob, umPos);
    }
};

class WrapAddIncoming2BaseKnot: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const idNob { ScrReadNobId(script) };
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->AddIncoming2BaseKnot(idNob, umPos);
    }
};

class WrapAddOutgoing2Pipe: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const idNob { ScrReadNobId(script) };
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->AddOutgoing2Pipe(idNob, umPos);
    }
};

class WrapAddIncoming2Pipe: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId         const idNob { ScrReadNobId(script) };
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->AddIncoming2Pipe(idNob, umPos);
    }
};

class WrapNewInputNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->NewInputNeuron(umPos);
    }
};

class WrapNewOutputNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->NewOutputNeuron(umPos);
    }
};

class WrapUndoCommand: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->UndoCommand();
    }
};

class WrapRedoCommand: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->RedoCommand();
    }
};

class WrapBreak : public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        int x = 42;
    }
};

void DefineNNetWrappers
(
    NNetModelReaderInterface * const pNMRI,
    NNetModelCommands        * const pCommands,
    NNetModelImporter        * const pModelImporter
)
{
    m_pNMRI          = pNMRI;
    m_pCommands      = pCommands;
    m_pModelImporter = pModelImporter;

    DEF_FUNC(AddModel);
    DEF_FUNC(AddSignal);
    DEF_FUNC(AnalyzeAnomalies);   
    DEF_FUNC(AnalyzeLoops);       
    DEF_FUNC(AppendInputNeuron);  
    DEF_FUNC(AppendOutputNeuron);  
    DEF_FUNC(AddIncoming2BaseKnot);   
    DEF_FUNC(AddIncoming2Pipe);   
    DEF_FUNC(AddOutgoing2Knot);   
    DEF_FUNC(AddOutgoing2Pipe);   
    DEF_FUNC(ClearBeepers);       
    DEF_FUNC(Connect);
    DEF_FUNC(CopySelection);      
    DEF_FUNC(CreateInitialNobs);
    DEF_FUNC(DeleteSelection);    
    DEF_FUNC(DeleteNob);        
    DEF_FUNC(DiscBaseKnot);
    DEF_FUNC(DiscIoConnector);         
    DEF_FUNC(Include);
    DEF_FUNC(InsertNeuron);       
    DEF_FUNC(MoveSelection);      
    DEF_FUNC(MoveSensor);      
    DEF_FUNC(MoveNob);          
    DEF_FUNC(NewInputNeuron);     
    DEF_FUNC(NewOutputNeuron);     
    DEF_FUNC(MakeIoConnector);
    DEF_FUNC(ResetModel);         
    DEF_FUNC(SelectAll);          
    DEF_FUNC(SelectAllBeepers);   
    DEF_FUNC(SelectNob);        
    DEF_FUNC(SelectNobsInRect); 
    DEF_FUNC(SelectSubtree);      
    DEF_FUNC(SetParameter);    
    DEF_FUNC(SetPulseRate); 
    DEF_FUNC(SetTriggerSound);
    DEF_FUNC(SplitNeuron);
    DEF_FUNC(ToggleStopOnTrigger);
    DEF_FUNC(UndoCommand);
    DEF_FUNC(RedoCommand);
    DEF_FUNC(Break);

    SymbolTable::ScrDefConst(L"ANALYZE_LOOPS",        static_cast<long>(IDM_ANALYZE_LOOPS      ));
    SymbolTable::ScrDefConst(L"ANALYZE_ANOMALIES",    static_cast<long>(IDM_ANALYZE_ANOMALIES  ));
    SymbolTable::ScrDefConst(L"DELETE_SELECTION",     static_cast<long>(IDM_DELETE_SELECTION   ));
    SymbolTable::ScrDefConst(L"CLEAR_BEEPERS",        static_cast<long>(IDM_CLEAR_BEEPERS      ));
    SymbolTable::ScrDefConst(L"SELECT_ALL_BEEPERS",   static_cast<long>(IDM_SELECT_ALL_BEEPERS ));
    SymbolTable::ScrDefConst(L"COPY_SELECTION",       static_cast<long>(IDM_COPY_SELECTION     ));
    SymbolTable::ScrDefConst(L"INSERT_NEURON",        static_cast<long>(IDD_INSERT_NEURON      ));
    SymbolTable::ScrDefConst(L"NEW_INPUT_NEURON",     static_cast<long>(IDD_NEW_INPUT_NEURON   ));
    SymbolTable::ScrDefConst(L"NEW_OUTPUT_NEURON",    static_cast<long>(IDD_NEW_OUTPUT_NEURON  ));
    SymbolTable::ScrDefConst(L"APPEND_INPUT_NEURON",  static_cast<long>(IDD_APPEND_INPUT_NEURON));
    SymbolTable::ScrDefConst(L"APPEND_OUTPUT_NEURON", static_cast<long>(IDD_APPEND_OUTPUT_NEURON));
    SymbolTable::ScrDefConst(L"ADD_OUTGOING2KNOT",    static_cast<long>(IDD_ADD_OUTGOING2KNOT  ));
    SymbolTable::ScrDefConst(L"ADD_INCOMING2BASEKNOT",static_cast<long>(IDD_ADD_INCOMING2BASEKNOT));
    SymbolTable::ScrDefConst(L"ADD_OUTGOING2PIPE",    static_cast<long>(IDD_ADD_OUTGOING2PIPE  ));
    SymbolTable::ScrDefConst(L"ADD_INCOMING2PIPE",    static_cast<long>(IDD_ADD_INCOMING2PIPE  ));

    ParamType::Apply2GlobalParameters
    ( 
        [](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst(ParamType::GetName(param), static_cast<unsigned long>(param));
        }
   );
}
