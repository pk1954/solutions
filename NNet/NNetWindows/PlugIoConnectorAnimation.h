// PlugIoConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "IoConnector.h"
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
        NobId                      idAnimated,
        NobId                      idTarget,
        MainWindow               & win
    )
      : AnimationSequence(win)
    {
        IoConnector & nobAnimated { * nmwi.GetNobPtr<IoConnector *>(idAnimated) };
        IoConnector & nobTarget   { * nmwi.GetNobPtr<IoConnector *>(idTarget) };
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(win, nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoConnectors>(nobAnimated, nobTarget, win));
    }

    virtual ~PlugIoConnectorAnimation() {};
};