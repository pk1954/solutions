// PlugConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "AnimationSequence.h"
#include "SingleNobAnimation.h"
#include "PlugConnectors.h"

class NNetModelWriterInterface;

class PlugConnectorAnimation : public AnimationSequence
{
public:
    PlugConnectorAnimation
    (
        NNetModelWriterInterface & nmwi,
        Connector                & nobAnimated,
        Connector                & nobTarget,
        MainWindow               & win
    )
        : AnimationSequence(win)
    {
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugConnectors>(nmwi, nobAnimated, nobTarget, win));
    }

    virtual ~PlugConnectorAnimation() {};
};