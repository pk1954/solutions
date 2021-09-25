// PlugIoConnectorAnimation.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "IoConnector.h"
#include "NNetCommand.h"
#include "SingleNobAnimation.h"
#include "PlugIoConnectors.h"

class RootWindow;
class NNetModelWriterInterface;

class PlugIoConnectorAnimation : public NNetCommand
{
public:
    PlugIoConnectorAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
    {
        IoConnector & nobAnimated { * m_pNMWI->GetNobPtr<IoConnector *>(idAnimated) };
        IoConnector & nobTarget   { * m_pNMWI->GetNobPtr<IoConnector *>(idTarget) };
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoConnectors  >(nobAnimated, nobTarget));
    }

    virtual ~PlugIoConnectorAnimation() {};
};