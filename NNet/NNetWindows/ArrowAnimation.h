// ArrowAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

using std::function;

class ArrowAnimation : public NNetCommand
{
    using ANIM_TYPE = MicroMeter;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    ArrowAnimation
    (
        ANIM_TYPE       & animated,
        ANIM_TYPE const & target
    )
      : m_animated(animated),
        m_start(animated),
        m_target(target)
    {
        m_upAnimation = make_unique<ANIMATION>(m_applicationFunc);
    }

    virtual void Do()
    {
        m_upAnimation->Start(m_animated, m_target);
    }

    virtual void Undo()
    {
        m_upAnimation->Start(m_animated, m_start);
    }

    virtual void UpdateUI()
    {
        m_animated = m_upAnimation->GetActual();
        AnimationCmd::UpdateUI();
    }

private:

    ANIM_TYPE           & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};