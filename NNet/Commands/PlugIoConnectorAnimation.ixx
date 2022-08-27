// PlugIoConnectorAnimation.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "Nob.h"
#include "IoConnector.h"

export module PlugIoConnectorAnimation;

import MoreTypes;
import SingleNobAnimation;
import PlugIoConnectors;
import NNetCommand;

export class PlugIoConnectorAnimation : public NNetCommand
{
public:
    PlugIoConnectorAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
    {
        IoConnector& nobAnimated{ *m_pNMWI->GetNobPtr<IoConnector*>(idAnimated) };
        IoConnector& nobTarget{ *m_pNMWI->GetNobPtr<IoConnector*>(idTarget) };
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 5.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(nobAnimated, CalcOffsetPosDir(nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoConnectors  >(nobAnimated, nobTarget));
    }

    virtual ~PlugIoConnectorAnimation() {};

    virtual bool IsAsyncCommand()
    {
        return true;
    };
};