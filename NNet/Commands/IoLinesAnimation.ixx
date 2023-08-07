// IoLinesAnimation.ixx
//
// Commands

module;

#include <vector>
#include <memory>

export module NNetCommands:IoLinesAnimation;

import Types;
import Command;
import Animation;
import NNetModel;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export class IoLinesAnimation : public Command
{
    using ANIM_TYPE = MicroMeterPntVector;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    IoLinesAnimation
    (
        vector<IoLine *> const & animated,
        ANIM_TYPE        const & target
    )
        : m_animated(animated),
        m_start(animated),
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
        for (size_t i = 0; i < m_animated.size(); ++i)
            m_animated.at(i)->SetPosDir(m_upAnimation->GetActual().GetPosDir(i));
        Command::UpdateUI();
    }

    bool IsAsyncCommand() final
    { 
        return true; 
    };

private:

    vector<IoLine *>      m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};
