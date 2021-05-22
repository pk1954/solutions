// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "AnimatedCommand.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "BaseKnotAnimation.h"
#include "ConnectIoNeuronsCommand.h"

using std::unique_ptr;

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
    unique_ptr<BaseKnotAnimation>       m_pBaseKnotAnimation;

    virtual void nextAnimationPhase(Mode const);
    virtual void updateUI();
};