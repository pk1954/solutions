// CoordAnimationCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:CoordAnimationCmd;

import Types;
import Commands;
import Uniform2D;
import Animation;
import :NNetCommand;

export class CoordAnimationCmd : public NNetCommand
{
    using ANIM_TYPE = Uniform2D<MicroMeter>;
    using ANIMATION = Animation<ANIM_TYPE>;
public:
    CoordAnimationCmd
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
        m_pStack->PushCommand(make_unique<CoordAnimationCmd>(animated, target));
    }

private:

    inline static const wstring NAME { L"CoordAnimation" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
//            CoordAnimationCmd::Push(ScrReadNobId(script));  //TODO
        }
    } m_wrapper;

    ANIM_TYPE           & m_animated;
    ANIM_TYPE       const m_start;
    ANIM_TYPE       const m_target;
    unique_ptr<ANIMATION> m_upAnimation;
};