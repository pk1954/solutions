// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "PluginAnimation.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "IoNeuron.h"
#include "SingleNobAnimation.h"
#include "PluginAnimation.h"
#include "ConnectIoObjectsCommand.h"

using std::unique_ptr;

using ConnectIoNeuronsCommand = ConnectIoObjectsCommand<IoNeuron,Neuron>;

class PluginIoNeuronAnimation : public PluginAnimation
{
public:
    PluginIoNeuronAnimation
    (
        IoNeuron    & nobAnimated,
        IoNeuron    & nobTarget,
        MainWindow  & win,
        WinCommands & cmds
    )
        : PluginAnimation(nobAnimated, nobTarget, win, cmds)
    {
        SetConnectionCommand(move(make_unique<ConnectIoNeuronsCommand>(nobAnimated, nobTarget)));

        SetTarget(3.0f);
        SetTarget(1.4f);
    }
    virtual ~PluginIoNeuronAnimation() {};

private:
};