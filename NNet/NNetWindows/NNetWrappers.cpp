// NNetWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

module;

#include <compare>

module NNetWin32:NNetWrappers;

import Script;
import BreakCommand;
import IncludeCommand;
import NNetModelIO;
import NNetCommands;

void InitializeNNetWrappers
(
    NNetModelIO   * const pModelIO,
    ScriptFunctor * const pScriptHook
)
{
    AddMicroSensorCmd::Register();
    AddModuleCommand::Register(pModelIO);
    AddNobsCommand::Register();
    AddSensorCmd::Register();
    AddSigGen2MonitorCmd::Register();
    AttachSigGen2ConCmd::Register();
    AttachSigGen2LineCmd::Register();
    BreakCommand::Register();
    ConnAnimationCommand::Register();
    Connect2NeuronCommand::Register();
    ConnectCreateForkCmd::Register();
    ConnectCreateSynapseCmd::Register();
    CoordAnimationCmd::Register();
    CreateForkCommand::Register();
    CreateInitialNobsCmd::Register();
    CreateSynapseCommand::Register();
    DeleteNobCommand::Register();
    DeleteSelectionCommand::Register();
    DeleteTrackCommand::Register();
    DelSensorCmd::Register();
    DelMicroSensorCmd::Register();
    DeselectModuleCmd::Register();
    DiscIoConnectorCmd::Register();
    ExtendInputLineCmd::Register();
    ExtendOutputLineCmd::Register();
    IncludeCommand::Register(*pScriptHook);
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
    RedoCommand::Register();
    RenameSigGenCmd::Register();
    ResetModelCmd::Register();
    RotateModelCommand::Register();
    RotateNobCommand::Register();
    RotateSelectionCommand::Register();
    SelectAllConnectedCmd::Register();
    SetActiveSigGenCmd::Register();
    SetNobCommand::Register();
    SetParameterCommand::Register();
    SetScalesCmd::Register();
    SetSigGenStaticDataCmd::Register();
    SizeSelectionCmd::Register();
    SizeSensorCmd::Register();
    SplitNeuronCmd::Register();
    StartStimulusCmd::Register();
    ToggleEmphModeCmd::Register();
    ToggleStopOnTriggerCmd::Register();
    UndoCommand::Register();
}
