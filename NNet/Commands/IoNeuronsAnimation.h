// IoNeuronsAnimation.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "NNetCommand.h"

using std::function;

class IoNeuronsAnimation : public NNetCommand
{
    using ANIM_TYPE = MicroMeterPntVector;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    IoNeuronsAnimation
    (
        vector<IoNeuron *> & animated,
        ANIM_TYPE    const & target
    )
      : m_animated(animated),
        m_start(animated),
        m_target(target)
    {
        m_upAnimation = make_unique<ANIMATION>(this);
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(m_animated, m_target));
    }

    virtual void Do()
    {
        for (auto & it: m_animated)
            it->LockDirection(); 
        m_upAnimation->Start(m_animated, m_target);
    }

    virtual void Undo()
    {
        m_upAnimation->Start(m_animated, m_start);
    }

    virtual void UpdateUI()
    {
        for (size_t i = 0; i < m_animated.size(); ++i)
            m_animated.at(i)->SetPosDir(m_upAnimation->GetActual().GetPosDir(i));
        AnimationCmd::UpdateUI();
    }

private:

    vector<IoNeuron *>    m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unsigned int          m_uiNrOfSteps;
    unique_ptr<ANIMATION> m_upAnimation;
};