// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "AnimatedCommand.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "SingleNobAnimation.h"
#include "ConnectIoObjectsCommand.h"

using std::unique_ptr;

using ConnectIoNeuronsCommand = ConnectIoObjectsCommand<IoNeuron,Neuron>;

class PluginIoNeuronAnimation : public AnimatedCommand
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

    IoNeuron & m_nobTarget;
    IoNeuron & m_nobAnimated;

    unique_ptr<ConnectIoNeuronsCommand> m_upConnectIoNeurons;
    unique_ptr<SingleNobAnimation>      m_upSingleNobAnimation;

    virtual void doPhase  ();
    virtual void undoPhase();
    virtual void updateUI ();
};