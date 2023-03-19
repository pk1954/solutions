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
import DrawContext;
import NNetModelCommands;
import Script;
import NNetModelIO;
import NNetModel;

import AddMicroSensorCmd;
import AddNobsCommand;
import AddSensorCmd;
import AddSigGen2MonitorCmd;
import AttachSigGen2ConCmd;
import AttachSigGen2LineCmd;
import ConnAnimationCommand;
import Connect2NeuronCommand;
import ConnectCreateForkCmd;
import ConnectCreateSynapseCmd;
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
import PlugIoConnectorsCmd;
import PlugIoLinesCmd;
import RenameSigGenCmd;
import ResetModelCmd;
import RotateModelCommand;
import RotateNobCommand;
import RotateSelectionCommand;
import SelectAllConnectedCmd;
import SetParameterCommand;
import SetActiveSigGenCmd;
import SizeSelectionCmd;
import SizeSensorCmd;
import SplitNeuronCmd;
import ToggleEmphModeCmd;
import ToggleStopOnTriggerCmd;

import :NNetInputOutputUI;

using std::wstring;

static NNetModelReaderInterface * m_pNMRI;
static NNetModelCommands        * m_pCommands;
static NNetModelIO              * m_pModelIO;

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
    AddSigGen2MonitorCmd::Register();
    AttachSigGen2ConCmd::Register();
    AttachSigGen2LineCmd::Register();
    ConnAnimationCommand::Register();
    Connect2NeuronCommand::Register();
    ConnectCreateForkCmd::Register();
    ConnectCreateSynapseCmd::Register();
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
    PlugIoConnectorsCmd::Register();
    PlugIoLinesCmd::Register();
    RenameSigGenCmd::Register();
    ResetModelCmd::Register();
    RotateModelCommand::Register();
    RotateNobCommand::Register();
    RotateSelectionCommand::Register();
    SelectAllConnectedCmd::Register();
    SetActiveSigGenCmd::Register();
    SetParameterCommand::Register();
    SizeSelectionCmd::Register();
    SizeSensorCmd::Register();
    SplitNeuronCmd::Register();
    ToggleEmphModeCmd::Register();
    ToggleStopOnTriggerCmd::Register();

    SymbolTable::ScrDefConst(L"AddModel",    new WrapAddModel);
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
