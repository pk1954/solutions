// ArrowAnimation.ixx
//
// Commands

module;

#include <functional>
#include "NNetModelWriterInterface.h"

export module ArrowAnimation;

import MoreTypes;
import Command;
import Animation;
import NNetParameters;

using std::unique_ptr;

export class ArrowAnimation : public Command
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
        m_upAnimation = make_unique<ANIMATION>(this);
    }

    void Do() final
    {
        m_upAnimation->Start(m_animated, m_target);
    }

    void Undo() final
    {
        m_upAnimation->Start(m_animated, m_start);
    }

    virtual void UpdateUI()
    {
        m_animated = m_upAnimation->GetActual();
        Command::UpdateUI();
    }

    virtual bool IsAsyncCommand() 
    { 
        return true; 
    };

private:

    ANIM_TYPE           & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};