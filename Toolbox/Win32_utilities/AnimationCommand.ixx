// AnimationCommand.ixx
//
// Toolbox\Win32_utilities

module;

#include <memory>
#include <iostream>

export module AnimationCommand;

import Animation;
import WinCommand;

export import Commands;

using std::unique_ptr;

export using std::wstring;
export using std::make_unique;

export template <typename ANIM_TYPE>
class AnimationCommand : public WinCommand
{
public:
    AnimationCommand
    (
        ANIM_TYPE         & animated,
        ANIM_TYPE    const& start,
        ANIM_TYPE    const& target
    )
      : m_animated(animated),
        m_start (start),
        m_target(target)
    {
        m_upAnimation = make_unique<Animation<ANIM_TYPE>>(this);
    }

    void SetNrOfSteps(unsigned int const  uiNrOfSteps)
    {
        m_upAnimation->SetNrOfSteps(uiNrOfSteps);
    }

    virtual void UpdateUI()
    {
        m_upAnimation->Update();
    }

    void Do() final
    {
        m_upAnimation->Start(&m_animated, m_start, m_target);
    }

    void Undo() final
    {
        m_upAnimation->Start(&m_animated, m_target, m_start);
    }

    bool IsAsyncCommand() final
    {
        return true;
    };

private:

    ANIM_TYPE     & m_animated;
    ANIM_TYPE const m_start;
    ANIM_TYPE const m_target;

    unique_ptr<Animation<ANIM_TYPE>> m_upAnimation;
};