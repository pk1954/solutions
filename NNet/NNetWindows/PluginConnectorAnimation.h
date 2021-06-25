// PluginConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "AnimationSequence.h"
#include "SingleNobAnimation.h"
#include "ConnectIoObjectsCommand.h"

class NNetModelWriterInterface;

class PluginConnectorAnimation : public AnimationSequence
{
public:
    PluginConnectorAnimation
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
        AddPhase(make_unique<ConnectIoObjectsCommand<Connector,ClosedConnector>>(nmwi, nobAnimated, nobTarget, win));
    }

    virtual ~PluginConnectorAnimation() {};

private:

};