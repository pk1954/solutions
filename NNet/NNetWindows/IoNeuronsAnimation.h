// IoNeuronsAnimation.h
//
// NNetWindows

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
        m_upAnimation = make_unique<ANIMATION>(m_applicationFunc);
    }

    virtual void Do()
    {
        ANIM_TYPE const actual(m_animated);
        for (auto & it: m_animated)
            it->LockDirection(); 
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(actual, m_target));
        m_upAnimation->Start(actual, m_target);
    }

    virtual void Undo()
    {
        ANIM_TYPE const actual(m_animated);
        m_upAnimation->SetNrOfSteps(CalcNrOfSteps(actual, m_start));
        m_upAnimation->Start(actual, m_start);
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
    unique_ptr<ANIMATION> m_upAnimation;
};