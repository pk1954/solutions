// IoLinesAnimation.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"

import MoreTypes;
import Command;
import Animation;

using std::vector;
using std::make_unique;

class IoLinesAnimation : public Command
{
    using ANIM_TYPE = MicroMeterPntVector;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    IoLinesAnimation
    (
        vector<IoLine *> const & animated,
        ANIM_TYPE          const & target
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
        for (auto & it: m_animated)
            it->LockDirection(); 
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

    vector<IoLine *>    m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unsigned int          m_uiNrOfSteps;
    unique_ptr<ANIMATION> m_upAnimation;
};