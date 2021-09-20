// PlugIoConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "IoConnector.h"
#include "win32_animationCmd.h"
#include "SingleNobAnimation.h"
#include "PlugIoConnectors.h"

class RootWindow;
class NNetModelWriterInterface;

class PlugIoConnectorAnimation : public AnimationCmd
{
public:
    PlugIoConnectorAnimation
    (
        NNetModelWriterInterface & nmwi,
        NobId                      idAnimated,
        NobId                      idTarget
    )
    {
        IoConnector & nobAnimated { * nmwi.GetNobPtr<IoConnector *>(idAnimated) };
        IoConnector & nobTarget   { * nmwi.GetNobPtr<IoConnector *>(idTarget) };
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoConnectors  >(nobAnimated, nobTarget));
    }

    virtual ~PlugIoConnectorAnimation() {};
};