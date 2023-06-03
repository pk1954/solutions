// ScalesAnimationCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:ScalesAnimationCmd;

import :AnimationCmd;

using fPixelPointAnimationCmd = AnimationCmd<fPixelPoint>;

export class ScalesAnimationCmd : public fPixelPointAnimationCmd
{
public:
    using fPixelPointAnimationCmd::fPixelPointAnimationCmd;

    virtual void UpdateUI()
    {
        fPixelPointAnimationCmd::UpdateUI();
        PostCmd2MainWin(IDD_ADJUST_SCALES, 0);
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        fPixelPoint& umAnimated,
        bool const   bOn,
        bool const   bAnimation
    )
    {
        fPixelPoint const umTarget { bOn ? fPixelPoint(35._fPixel, 30._fPixel) : fPP_ZERO };

        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        if (bAnimation)
            PushCommand(make_unique<ScalesAnimationCmd>(umAnimated, umTarget));
        else
        {
            umAnimated = umTarget;
            PostCmd2MainWin(IDD_ADJUST_SCALES, 0);
        }
    }

private:

    inline static const wstring NAME { L"ScalesAnimation" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            fPixelPoint animated { ScrReadfPixelPoint(script) };
            bool       bOn       { script.ScrReadBool() };
            ScalesAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper;
};