// PlugIoConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "IoConnector.h"
#include "ClosedConnector.h"
#include "AnimationSequence.h"
#include "SingleNobAnimation.h"
#include "PlugIoConnectors.h"

class NNetModelWriterInterface;

class PlugIoConnectorAnimation : public AnimationSequence
{
public:
    PlugIoConnectorAnimation
    (
        NNetModelWriterInterface & nmwi,
        IoConnector              & nobAnimated,
        IoConnector              & nobTarget,
        MainWindow               & win
   )
        : AnimationSequence(win)
    {
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoConnectors>(nmwi, nobAnimated, nobTarget, win));
    }

    virtual ~PlugIoConnectorAnimation() {};
};