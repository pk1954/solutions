// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "PluginAnimation.h"
#include "ConnectIoObjectsCommand.h"

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
        SetConnectionCommand(move(make_unique<ConnectIoObjectsCommand<IoNeuron,Neuron>>(nobAnimated, nobTarget)));

        SetTarget(3.0f);
        SetTarget(1.4f);
    }
    virtual ~PluginIoNeuronAnimation() {};

private:
};