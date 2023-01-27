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

import AddNobsCommand;
import AddSensorSignalCmd;
import ConnAnimationCommand;
import ConnectCreateForkCmd;
import ConnSynapse2NewPipeCmd;
import CreateForkCommand;
import CreateInitialNobsCmd;
import CreateSynapseCommand;
import DeselectModuleCmd;
import DiscIoConnectorCmd;
import ExtendInputLineCmd;
import ExtendOutputLineCmd;
import InsertTrackCommand;
import MoveNobCommand;
import MoveSelectionCommand;
import MoveSensorCmd;
import MoveSignalCmd;
import NewIoLinePairCmd;
import ResetModelCmd;
import SelectAllConnectedCmd;
import SplitNeuronCmd;

using std::wstring;

static NNetModelReaderInterface * m_pNMRI;
static NNetModelCommands        * m_pCommands;
static NNetModelIO              * m_pModelIO;

class WrapConnect: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId          const idSrc { ScrReadNobId(script) };
        NobId          const idDst { ScrReadNobId(script) };
        ConnectionType const ctype { m_pNMRI->ConnectionResult(idSrc, idDst) };
        if (ctype != ConnectionType::ct_none)
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

class WrapToggleStopOnTrigger: public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
        NobId const id { ScrReadNobId(script) };
        m_pCommands->ToggleStopOnTrigger(id);
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
        m_pCommands->DeleteNob(ScrReadNobId(script));
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

    AddNobsCommand::Register();
    AddSensorSignalCmd::Register();
    ConnAnimationCommand::Register();
    ConnectCreateForkCmd::Register();
    ConnSynapse2NewPipeCmd::Register();
    CreateForkCommand::Register();
    CreateInitialNobsCmd::Register();
    CreateSynapseCommand::Register();
    DeselectModuleCmd::Register();
    DiscIoConnectorCmd::Register();
    ExtendInputLineCmd::Register();
    ExtendOutputLineCmd::Register();
    InsertTrackCommand::Register();
    MoveNobCommand::Register();
    MoveSelectionCommand::Register();
    MoveSensorCmd::Register();
    MoveSignalCmd::Register();
    NewIoLinePairCmd::Register();
    ResetModelCmd::Register();
    SelectAllConnectedCmd::Register();
    SplitNeuronCmd::Register();

    SymbolTable::ScrDefConst(L"AddModel",            new WrapAddModel);
    SymbolTable::ScrDefConst(L"Connect",             new WrapConnect );
    SymbolTable::ScrDefConst(L"DeleteSelection",     new WrapDeleteSelection );
    SymbolTable::ScrDefConst(L"DeleteNob",           new WrapDeleteNob );
    SymbolTable::ScrDefConst(L"Include",             new WrapInclude );
    SymbolTable::ScrDefConst(L"SetParameter",        new WrapSetParameter);
    SymbolTable::ScrDefConst(L"ToggleStopOnTrigger", new WrapToggleStopOnTrigger );
    SymbolTable::ScrDefConst(L"UndoCommand",         new WrapUndoCommand );
    SymbolTable::ScrDefConst(L"RedoCommand",         new WrapRedoCommand );
    SymbolTable::ScrDefConst(L"Break",               new WrapBreak );

    
    //template InsertPosNobCommand
    SymbolTable::ScrDefConst(L"InsertNeuron", new WrapInsertNeuron);

    ParamType::Apply2GlobalParameters
    ( 
        [](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst(ParamType::GetName(param), static_cast<unsigned long>(param));
        }
   );
}
