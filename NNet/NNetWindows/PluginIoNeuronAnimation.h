// PluginIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "AnimationSequence.h"
#include "ConnectIoObjectsCommand.h"

class NNetModelWriterInterface;

class PluginIoNeuronAnimation : public AnimationSequence
{
public:
    PluginIoNeuronAnimation
    (
        NNetModelWriterInterface & nmwi,
        IoNeuron    & nobAnimated,
        IoNeuron    & nobTarget,
        MainWindow  & win
    )
        : AnimationSequence(win)
    {
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<ConnectIoObjectsCommand<IoNeuron,Neuron>>(nmwi, nobAnimated, nobTarget, win));
    }

    virtual ~PluginIoNeuronAnimation() {};

private:

};