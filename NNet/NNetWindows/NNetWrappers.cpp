// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

module;

#include <string>
#include "Resource.h"

module NNetWrappers;

import Types;
import ErrHndl;
import SoundInterface;
import UtilityWrappers;
import Symtab;
import NNetInputOutputUI;
import DrawContext;
import NNetModelCommands;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

using std::wstring;

static NNetModelReaderInterface * m_pNMRI;
static NNetModelCommands        * m_pCommands;
static NNetModelIO              * m_pModelIO;

class WrapConnect: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId          const idSrc { m_pCommands->GetHighlightedNob() };
        NobId          const idDst { m_pCommands->GetTargetNob() };
        ConnectionType const ctype { m_pNMRI->ConnectionResult(idSrc, idDst) };
        if (ctype != ConnectionType::ct_none)
        {
            m_pCommands->Connect();
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
        m_pModelIO->Import
        (
            script.ScrReadString(), 
            NNetInputOutputUI::CreateNew(IDM_ADD_IMPORTED_MODEL)
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
        m_pCommands->AddSensor(umCircle, trackNr);
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
        bool  const bOn { script.ScrReadInt() != 0 };
        m_pCommands->SelectSubtree(bOn);
    }
};

class WrapSelectNob: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        tBoolOp const op { ScrReadBoolOp(script) };
        m_pCommands->SelectNob(op);
    }
};

class WrapToggleStopOnTrigger: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->ToggleStopOnTrigger();
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
        m_pCommands->DeleteNob();
    }
};

class WrapDeleteBaseKnot: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->DeleteBaseKnot();
    }
};

class WrapDiscIoConnector: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->DiscIoConnector();
    }
};

class WrapSplitNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->SplitNeuron();
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
        m_pCommands->MoveNob(ScrReadMicroMeterPnt(script));
    }
};

class WrapMoveSensor: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        SensorId      const id      ( script.ScrReadUint() );
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
        m_pCommands->MoveSelection(ScrReadMicroMeterPnt(script));
    }
};

class WrapInsertNeuron: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->InsertNeuron(ScrReadMicroMeterPnt(script));
    }
};

class WrapExtendOutputLine: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->ExtendOutputLine(ScrReadMicroMeterPnt(script));
    }
};

class WrapExtendInputLine : public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->ExtendInputLine(ScrReadMicroMeterPnt(script));
    }
};

class WrapAddOutgoing2Pipe: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->AddOutgoing2Pipe(ScrReadMicroMeterPnt(script));
    }
};

class WrapAddIncoming2Pipe: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        m_pCommands->AddIncoming2Pipe(ScrReadMicroMeterPnt(script));
    }
};

class WrapNewIoLinePair: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
        m_pCommands->NewIoLinePair(umPos);
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

void NNetWrappersSetModelInterface(NNetModelReaderInterface * const pNMRI)
{
    m_pNMRI = pNMRI;
};

void InitializeNNetWrappers
(
    NNetModelCommands * const pCommands,
    NNetModelIO       * const pModelIO
)
{
    m_pCommands = pCommands;
    m_pModelIO  = pModelIO;

    SymbolTable::ScrDefConst(L"AddModel",            new WrapAddModel);
    SymbolTable::ScrDefConst(L"AddSignal",           new WrapAddSignal );
    SymbolTable::ScrDefConst(L"AnalyzeAnomalies",    new WrapAnalyzeAnomalies); 
    SymbolTable::ScrDefConst(L"AnalyzeLoops",        new WrapAnalyzeLoops); 
    SymbolTable::ScrDefConst(L"ExtendInputLine",     new WrapExtendInputLine);
    SymbolTable::ScrDefConst(L"ExtendOutputLine",    new WrapExtendOutputLine);
    SymbolTable::ScrDefConst(L"AddIncoming2Pipe",    new WrapAddIncoming2Pipe); 
    SymbolTable::ScrDefConst(L"AddOutgoing2Pipe",    new WrapAddOutgoing2Pipe); 
    SymbolTable::ScrDefConst(L"Connect",             new WrapConnect );
    SymbolTable::ScrDefConst(L"CopySelection",       new WrapCopySelection );
    SymbolTable::ScrDefConst(L"CreateInitialNobs",   new WrapCreateInitialNobs );
    SymbolTable::ScrDefConst(L"DeleteSelection",     new WrapDeleteSelection );
    SymbolTable::ScrDefConst(L"DeleteNob",           new WrapDeleteNob );
    SymbolTable::ScrDefConst(L"DeleteBaseKnot",      new WrapDeleteBaseKnot);
    SymbolTable::ScrDefConst(L"DiscIoConnector",     new WrapDiscIoConnector ); 
    SymbolTable::ScrDefConst(L"Include",             new WrapInclude );
    SymbolTable::ScrDefConst(L"InsertNeuron",        new WrapInsertNeuron); 
    SymbolTable::ScrDefConst(L"MoveSelection",       new WrapMoveSelection );
    SymbolTable::ScrDefConst(L"MoveSensor",          new WrapMoveSensor);
    SymbolTable::ScrDefConst(L"MoveNob",             new WrapMoveNob );
    SymbolTable::ScrDefConst(L"NewIoLinePair",       new WrapNewIoLinePair ); 
    SymbolTable::ScrDefConst(L"MakeIoConnector",     new WrapMakeIoConnector );
    SymbolTable::ScrDefConst(L"ResetModel",          new WrapResetModel); 
    SymbolTable::ScrDefConst(L"SelectAll",           new WrapSelectAll );
    SymbolTable::ScrDefConst(L"SelectNob",           new WrapSelectNob );
    SymbolTable::ScrDefConst(L"SelectNobsInRect",    new WrapSelectNobsInRect); 
    SymbolTable::ScrDefConst(L"SelectSubtree",       new WrapSelectSubtree );
    SymbolTable::ScrDefConst(L"SetParameter",        new WrapSetParameter);
    SymbolTable::ScrDefConst(L"SplitNeuron",         new WrapSplitNeuron );
    SymbolTable::ScrDefConst(L"ToggleStopOnTrigger", new WrapToggleStopOnTrigger );
    SymbolTable::ScrDefConst(L"UndoCommand",         new WrapUndoCommand );
    SymbolTable::ScrDefConst(L"RedoCommand",         new WrapRedoCommand );
    SymbolTable::ScrDefConst(L"Break",               new WrapBreak );

    SymbolTable::ScrDefConst(L"ANALYZE_LOOPS",       static_cast<long>(IDM_ANALYZE_LOOPS       ));
    SymbolTable::ScrDefConst(L"ANALYZE_ANOMALIES",   static_cast<long>(IDM_ANALYZE_ANOMALIES   ));
    SymbolTable::ScrDefConst(L"DELETE_SELECTION",    static_cast<long>(IDM_DELETE_SELECTION    ));
    SymbolTable::ScrDefConst(L"COPY_SELECTION",      static_cast<long>(IDM_COPY_SELECTION      ));
    SymbolTable::ScrDefConst(L"INSERT_NEURON",       static_cast<long>(IDD_INSERT_NEURON       ));
    SymbolTable::ScrDefConst(L"NEW_IO_LINE_PAIR",    static_cast<long>(IDD_NEW_IO_LINE_PAIR    ));
    SymbolTable::ScrDefConst(L"EXTEND_INPUTLINE",    static_cast<long>(IDD_EXTEND_INPUTLINE    ));
    SymbolTable::ScrDefConst(L"EXTEND_OUTPUTLINE",   static_cast<long>(IDD_EXTEND_OUTPUTLINE   ));
    SymbolTable::ScrDefConst(L"ADD_INCOMING2NEURON", static_cast<long>(IDD_ADD_INCOMING2NEURON ));
    SymbolTable::ScrDefConst(L"ADD_OUTGOING2PIPE",   static_cast<long>(IDD_ADD_OUTGOING2PIPE   ));
    SymbolTable::ScrDefConst(L"ADD_INCOMING2PIPE",   static_cast<long>(IDD_ADD_INCOMING2PIPE   ));

    ParamType::Apply2GlobalParameters
    ( 
        [](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst(ParamType::GetName(param), static_cast<unsigned long>(param));
        }
   );
}
