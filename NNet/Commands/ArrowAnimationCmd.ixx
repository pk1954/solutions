// ArrowAnimationCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:ArrowAnimationCmd;

import IoConstants;
import :AnimationCmd;

using MicroMeterAnimationCmd = AnimationCmd<MicroMeter>;

export class ArrowAnimationCmd : public MicroMeterAnimationCmd
{
public:
    using MicroMeterAnimationCmd::MicroMeterAnimationCmd;

    virtual void UpdateUI()
    {
        MicroMeterAnimationCmd::UpdateUI();
        Command::UpdateUI();
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        MicroMeter & umAnimated,
        bool const   bOn,
        bool const   bAnimation
    )
    {
        MicroMeter const umTarget { bOn ? 30.0_MicroMeter : 0._MicroMeter };

        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        if (bAnimation)
            PushCommand(make_unique<ArrowAnimationCmd>(umAnimated, umTarget));
        else
            umAnimated = umTarget;
    }

private:

    inline static const wstring NAME { L"ArrowAnimation" };

    inline static struct Wrapper : public WrapBase
    {
        using WrapBase::WrapBase;
        void operator() (Script& script) const final
        {
            MicroMeter animated { ScrReadMicroMeter(script) };
            bool       bOn      { script.ScrReadBool() };
            ArrowAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper { NAME };
};