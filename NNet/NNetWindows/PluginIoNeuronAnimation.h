// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "vector"
#include "MoreTypes.h"
#include "AnimatedCommand.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"

using std::vector;

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

    IoNeuron           & m_nobTarget;
    IoNeuron           & m_nobAnimated;
    unique_ptr<IoNeuron> m_upNobAnimated;
    unique_ptr<IoNeuron> m_upNobTarget;
    unique_ptr<Neuron>   m_upClosedNeuron {};

    virtual void nextAnimationPhase();
    virtual void updateUI();

    unsigned int const calcNrOfSteps(MicroMeterPosDir const &, MicroMeterPosDir const &) const;
};