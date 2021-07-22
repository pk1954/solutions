// PlugIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "SingleNobAnimation.h"
#include "AnimationSequence.h"
#include "PlugIoNeurons.h"

class NNetModelWriterInterface;

class PlugIoNeuronAnimation : public AnimationSequence
{
public:
    PlugIoNeuronAnimation
    (
        NNetModelWriterInterface & nmwi,
        IoNeuron                 & nobAnimated,
        IoNeuron                 & nobTarget,
        MainWindow               & win
    )
        : AnimationSequence(win)
    {
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoNeurons>(nmwi, nobAnimated, nobTarget, win));
    }

    virtual ~PlugIoNeuronAnimation() {};
};