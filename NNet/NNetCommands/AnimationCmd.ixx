// AnimationCmd.ixx
//
// NNetCommands

module;

#include <memory>
#include <iostream>

export module NNetCommands:AnimationCmd;

import Animation;
import :NNetCommand;

export template <typename ANIM_TYPE>
class AnimationCmd : public NNetCommand
{
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    AnimationCmd
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

    virtual void UpdateUI()
    {
        m_animated = m_upAnimation->GetActual();
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

    ANIM_TYPE           & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};