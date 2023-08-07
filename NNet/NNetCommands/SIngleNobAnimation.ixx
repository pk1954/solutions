// SingleNobAnimation.ixx
//
// NNetCommands

module;

//#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>

export module NNetCommands:SingleNobAnimation;

import Types;
import Command;
import Animation;
import NNetModel;

using std::max;
using std::function;
using std::make_unique;
using std::unique_ptr;

export class SingleNobAnimation : public Command
{
public:
    SingleNobAnimation
    (
        Nob                    & nob,
        MicroMeterPosDir const & target
    )
      : m_nob(nob),
        m_animated(m_nob.GetPosDir()),
        m_start(m_animated),
        m_target(target)
    {
        MicroMeterPosDir const umPosDirDiff    { m_animated - m_target };
        unsigned int     const uiStepsFromRot  { CalcNrOfSteps(Normalize(umPosDirDiff.GetDir())) };
        unsigned int     const uiStepsFromMove { CalcNrOfSteps(Hypot(umPosDirDiff.GetPos())) };
        unsigned int     const uiSteps         { max(uiStepsFromRot, uiStepsFromMove) + 1 };
        m_upAnimation = make_unique<Animation<MicroMeterPosDir>>(this);
        m_upAnimation->SetNrOfSteps(uiSteps);
    }

    void UpdateUI() final
    {
        m_nob.SetPosDir(m_animated);
        Command::UpdateUI();
    }

    void Do() final
    {
        m_upAnimation->Start(m_animated, m_target);
    }

    void Undo() final
    {
        m_upAnimation->Start(m_animated, m_start);
    }

    bool IsAsyncCommand() final
    { 
        return true; 
    };

private:

    Nob                                   & m_nob;
    MicroMeterPosDir                        m_animated;
    MicroMeterPosDir                  const m_start;
    MicroMeterPosDir                  const m_target;
    unique_ptr<Animation<MicroMeterPosDir>> m_upAnimation;
};