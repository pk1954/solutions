// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

module;

#include <string>
#include "Resource.h"

module NNetWin32:NNetWrappers;

import Types;
import ErrHndl;
import SoundInterface;
import UtilityWrappers;
import Symtab;
import DrawContext;
import NNetModelCommands;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

import AddMicroSensorCmd;
import AddNobsCommand;
import AddSensorCmd;
import AttachSigGen2ConCmd;
import AttachSigGen2LineCmd;
import ConnAnimationCommand;
import ConnectCreateForkCmd;
import ConnectCreateSynapseCmd;
//import ConnSynapse2NewPipeCmd;
import CreateForkCommand;
import CreateInitialNobsCmd;
import CreateSynapseCommand;
import DeleteNobCommand;
import DeleteSelectionCommand;
import DeleteTrackCommand;
import DeleteSensorCmd;
import DeleteSignalCommand;
import DelMicroSensorCmd;
import DeselectModuleCmd;
import DiscIoConnectorCmd;
import ExtendInputLineCmd;
import ExtendOutputLineCmd;
import InsertKnotCommand;
import InsertNeuronCommand;
import InsertTrackCommand;
import MoveNobCommand;
import MoveSelectionCommand;
import MoveSensorCmd;
import MoveSignalCmd;
import NewIoLinePairCmd;
import NewSigGenCmd;
import RenameSigGenCmd;
import ResetModelCmd;
import RotateModelCommand;
import RotateSelectionCommand;
import SelectAllConnectedCmd;
import SetParameterCommand;
import SizeSelectionCmd;
import SizeSensorCmd;
import SplitNeuronCmd;
import ToggleStopOnTriggerCmd;

import :NNetInputOutputUI;

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

    AddMicroSensorCmd::Register();
    AddNobsCommand::Register();
    AddSensorCmd::Register();
    AttachSigGen2ConCmd::Register();
    AttachSigGen2LineCmd::Register();
    ConnAnimationCommand::Register();
    ConnectCreateForkCmd::Register();
    ConnectCreateSynapseCmd::Register();
    //ConnSynapse2NewPipeCmd::Register();
    CreateForkCommand::Register();
    CreateInitialNobsCmd::Register();
    CreateSynapseCommand::Register();
    DeleteNobCommand::Register();
    DeleteSelectionCommand::Register();
    DeleteTrackCommand::Register();
    DeleteSensorCmd::Register();
    DeleteSignalCommand::Register();
    DelMicroSensorCmd::Register();
    DeselectModuleCmd::Register();
    DiscIoConnectorCmd::Register();
    ExtendInputLineCmd::Register();
    ExtendOutputLineCmd::Register();
    InsertKnotCommand::Register();
    InsertNeuronCommand::Register();
    InsertTrackCommand::Register();
    MoveNobCommand::Register();
    MoveSelectionCommand::Register();
    MoveSensorCmd::Register();
    MoveSignalCmd::Register();
    NewIoLinePairCmd::Register();
    NewSigGenCmd::Register();
    RenameSigGenCmd::Register();
    ResetModelCmd::Register();
    RotateModelCommand::Register();
    RotateSelectionCommand::Register();
    SelectAllConnectedCmd::Register();
    SetParameterCommand::Register();
    SizeSelectionCmd::Register();
    SizeSensorCmd::Register();
    SplitNeuronCmd::Register();
    ToggleStopOnTriggerCmd::Register();

    SymbolTable::ScrDefConst(L"AddModel",    new WrapAddModel);
    SymbolTable::ScrDefConst(L"Connect",     new WrapConnect );
    SymbolTable::ScrDefConst(L"Include",     new WrapInclude );
    SymbolTable::ScrDefConst(L"UndoCommand", new WrapUndoCommand );
    SymbolTable::ScrDefConst(L"RedoCommand", new WrapRedoCommand );
    SymbolTable::ScrDefConst(L"Break",       new WrapBreak );
   
    ParamType::Apply2GlobalParameters
    ( 
        [](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst(ParamType::GetName(param), static_cast<unsigned long>(param));
        }
   );
}
