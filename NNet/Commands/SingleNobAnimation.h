// SingleNobAnimation.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "win32_command.h"

using std::function;

class SingleNobAnimation : public Command
{
    using ANIM_TYPE = MicroMeterPosDir;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    SingleNobAnimation
    (
        Nob             & animated,
        ANIM_TYPE const & target
    )
      : m_animated(animated),
        m_start(animated.GetPosDir()),
        m_target(target)
    {
        m_upAnimation = make_unique<ANIMATION>(this);
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(m_animated, m_target));
    }

    void Do() final
    {
        m_upAnimation->Start(m_animated, m_target);
    }

    void Undo() final
    {
        m_upAnimation->Start(m_animated, m_start);
    }

    void UpdateUI() final
    {
        m_animated.SetPosDir(m_upAnimation->GetActual());
        Command::UpdateUI();
    }

    bool IsAsyncCommand() final
    { 
        return true; 
    };

private:

    Nob                 & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};