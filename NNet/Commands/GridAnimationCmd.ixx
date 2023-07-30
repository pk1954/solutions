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

    GridAnimationCmd
    (
        RootWindow * const pWin,
        float            & fAnimated,
        float      const & fTarget
    )
      : AnimationCmd<float>(fAnimated, fTarget),
        m_pWin(pWin)
    {}

    virtual void UpdateUI()
    {
        FloatAnimationCmd::UpdateUI();
        m_pWin->SendCommand(IDM_GRID_UPDATE);
    }

    //static void Register()
    //{
    //    SymbolTable::ScrDefConst(NAME, &m_wrapper);
    //}

    static void Push
    (
        RootWindow* const pWin,
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

        PushCommand(make_unique<GridAnimationCmd>(pWin, fAnimated, fTarget));
    }

private:

    inline static const wstring NAME { L"GridAnimation" };

    RootWindow * m_pWin { nullptr };

    //inline static struct Wrapper : public ScriptFunctor
    //{
    //    void operator() (Script& script) const final
    //    {
    //        float       fAnimated { Cast2Float(script.ScrReadFloat()) };
    //        float const fTarget   { Cast2Float(script.ScrReadFloat()) };
    //        GridAnimationCmd::Push(fAnimated, fTarget);
    //    }
    //} m_wrapper;
};