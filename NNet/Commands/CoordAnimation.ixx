// CoordAnimation.ixx
//
// Commands

module;

#include <memory>

export module CoordAnimation;

import Types;
import Commands;
import Uniform2D;
import Animation;

using std::unique_ptr;
using std::make_unique;

export class CoordAnimation : public Command
{
    using ANIM_TYPE = Uniform2D<MicroMeter>;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    CoordAnimation
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

    void UpdateUI() final
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