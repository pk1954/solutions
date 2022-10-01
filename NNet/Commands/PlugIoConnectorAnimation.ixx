// PlugIoConnectorAnimation.ixx
//
// Commands

module;

#include <memory>

export module PlugIoConnectorAnimation;

import Types;
import SingleNobAnimation;
import PlugIoConnectors;
import NNetCommand;
import NNetModel;

using std::make_unique;

export class PlugIoConnectorAnimation : public NNetCommand
{
public:
    PlugIoConnectorAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
    {
        IoConnector& nobAnimated { *m_pNMWI->GetNobPtr<IoConnector*>(idAnimated) };
        IoConnector& nobTarget   { *m_pNMWI->GetNobPtr<IoConnector*>(idTarget) };
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