// GridAnimationCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:GridAnimationCmd;

import SaveCast;
import Win32_Util_Resource;
import RootWindow;
import :AnimationCmd;

using std::wstring;

using FloatAnimationCmd = AnimationCmd<float>;

export class GridAnimationCmd : public FloatAnimationCmd
{
public:

    static void Initialize(RootWindow * const pWin)
    {
        m_pWin = pWin;
    }

    using FloatAnimationCmd::FloatAnimationCmd;

    virtual void UpdateUI()
    {
        FloatAnimationCmd::UpdateUI();
        m_pWin->SendCommand(IDM_GRID_UPDATE);
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        float     & fAnimated,
        float const fTarget
    )
    {
        if (fTarget == fAnimated)
            return;

        if (IsTraceOn()) 
        { 
            TraceStream()<< NAME << SPACE << fAnimated << SPACE << fTarget << SPACE; // << endl;
        }

        PushCommand(make_unique<GridAnimationCmd>(fAnimated, fTarget));
    }

private:

    inline static const wstring NAME { L"GridAnimation" };

    inline static RootWindow * m_pWin { nullptr };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            float       fAnimated { Cast2Float(script.ScrReadFloat()) };
            float const fTarget   { Cast2Float(script.ScrReadFloat()) };
            GridAnimationCmd::Push(fAnimated, fTarget);
        }
    } m_wrapper;
};