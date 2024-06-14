// SetGridCmd.ixx
//
// NNetCommands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:SetGridCmd;

import IoConstants;
import Win32_Util_Resource;
import RootWindow;
import WinManager;
import Commands;
import WrapSetGrid;
import :AnimationCmd;

using std::wstring;
using std::wostream;
using std::make_unique;

using FloatAnimationCmd = AnimationCmd<float>;

export class SetGridCmd : public FloatAnimationCmd
{
public:

    SetGridCmd
    (
        RootWindow * const pWin,
        float            & fAnimated,
        float      const & fTarget
    )
      : FloatAnimationCmd(fAnimated, fTarget),
        m_pWin(pWin)
    {}

    virtual void UpdateUI()
    {
        FloatAnimationCmd::UpdateUI();
        m_pWin->SendCommand(IDD_GRID_UPDATE);
    }

    static void Push
    (
        RootWindow &rootWin,
        float      &fAnimated,
        bool  const bActive,
        bool  const bAnimation
    )
    {
        float const fTarget { bActive ? 1.0f : 0.0f };
        if (fTarget == fAnimated)
            return;

        if (IsTraceOn())
            WrapSetGrid::WriteSetGrid(TraceStream(), rootWin, bActive);

        if (bAnimation)
        {
            PushCommand(make_unique<SetGridCmd>(&rootWin, fAnimated, fTarget));
        }
        else
        {
            fAnimated = fTarget;
            rootWin.SendCommand(IDD_GRID_UPDATE);
        }
    }

private:

    RootWindow * m_pWin { nullptr };
};