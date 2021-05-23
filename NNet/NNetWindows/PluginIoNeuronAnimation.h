// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "PluginAnimation.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "SingleNobAnimation.h"
#include "ConnectIoObjectsCommand.h"

using std::unique_ptr;

using ConnectIoNeuronsCommand = ConnectIoObjectsCommand<IoNeuron,Neuron>;

class PluginIoNeuronAnimation : public PluginAnimation
{
public:
    PluginIoNeuronAnimation
    (
        IoNeuron    &,
        IoNeuron    &,
        MainWindow  &,
        WinCommands &
    );
    virtual ~PluginIoNeuronAnimation() {};

private:
};