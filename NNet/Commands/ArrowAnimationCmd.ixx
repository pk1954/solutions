// ArrowAnimationCmd.ixx
//
// Commands

module;

#include <functional>
#include <memory>
#include <iostream>

export module NNetCommands:ArrowAnimationCmd;

import Types;
import IoUtil;
import Commands;
import Animation;
import NNetModel;
import :NNetCommand;

export class ArrowAnimationCmd : public NNetCommand
{
    using ANIM_TYPE = MicroMeter;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    ArrowAnimationCmd
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

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        ANIM_TYPE      & animated,
        ANIM_TYPE const& target
    )
    {
        if (IsTraceOn())
            TraceStream() << NAME << endl;
        PushCommand(make_unique<ArrowAnimationCmd>(animated, target));
    }

private:

    inline static const wstring NAME { L"ArrowAnimation" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
        }
    } m_wrapper;

    ANIM_TYPE           & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};