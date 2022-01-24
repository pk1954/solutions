// IoNeuronsAnimation.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "win32_command.h"

using std::function;

class IoNeuronsAnimation : public Command
{
    using ANIM_TYPE = MicroMeterPntVector;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    IoNeuronsAnimation
    (
        vector<IoNeuron *> const & animated,
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

    vector<IoNeuron *>    m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unsigned int          m_uiNrOfSteps;
    unique_ptr<ANIMATION> m_upAnimation;
};