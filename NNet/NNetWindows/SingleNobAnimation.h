// SingleNobAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "NNetCommand.h"

using std::function;

class SingleNobAnimation : public NNetCommand
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
        m_upAnimation = make_unique<ANIMATION>(m_applicationFunc);
    }

    virtual void Do()
    {
        //wcout << L'#' << __FUNCDNAME__ << endl;
        ANIM_TYPE const actual(m_animated);
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(actual, m_target));
        m_upAnimation->Start(actual, m_target);
    }

    virtual void Undo()
    {
        //wcout << L'#' << __FUNCDNAME__ << endl;
        ANIM_TYPE const actual(m_animated);
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(actual, m_start));
        m_upAnimation->Start(actual, m_start);
    }

    virtual void UpdateUI()
    {
        m_animated.SetPosDir(m_upAnimation->GetActual());
        AnimationCmd::UpdateUI();
    }

private:

    Nob                 & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};