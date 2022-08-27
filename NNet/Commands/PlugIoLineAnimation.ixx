// PlugIoLineAnimation.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "IoLine.h"

export module PlugIoLineAnimation;

import MoreTypes;
import SingleNobAnimation;
import PlugIoLines;
import NobId;
import NNetCommand;

using std::make_unique;

export class PlugIoLineAnimation : public NNetCommand
{
public:
    PlugIoLineAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
      : m_nobAnimated(*m_pNMWI->GetNobPtr<IoLine*>(idAnimated)),
        m_nobTarget  (*m_pNMWI->GetNobPtr<IoLine*>(idTarget))
    {
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoLines       >(m_nobAnimated, m_nobTarget));
    }

    ~PlugIoLineAnimation() final = default;

    void Do() final
    {
        m_nobAnimated.LockDirection();
        Command::Do();
    }

    void Undo() final
    {
        Command::Undo();
        m_nobAnimated.UnlockDirection();
    }

    bool IsAsyncCommand() final
    {
        return true;
    };

private:
    IoLine & m_nobAnimated;
    IoLine & m_nobTarget;
};