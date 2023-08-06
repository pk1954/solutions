// SetScalesCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:SetScalesCmd;

import IoConstants;
import Win32_Util_Resource;
import BaseWindow;
import WinManager;
import BoolWrapper;
import WrapSetScales;
import :AnimationCmd;

using std::wstring;

using fPixelPointAnimationCmd = AnimationCmd<fPixelPoint>;

export class SetScalesCmd : public fPixelPointAnimationCmd
{
public:
    SetScalesCmd
    (
        BaseWindow * const pWin,
        fPixelPoint      & fAnimated,
        fPixelPoint const& fTarget
    )
      : fPixelPointAnimationCmd(fAnimated, fTarget),
        m_pWin(pWin)
    {}

    virtual void UpdateUI()
    {
        fPixelPointAnimationCmd::UpdateUI();
        m_pWin->SendCommand(IDD_SCALES_UPDATE);
    }

    static void Push
    (
        BaseWindow  &baseWin,
        fPixelPoint &umAnimated,
        bool  const bActive,
        bool  const bAnimation
    )
    {
        fPixelPoint const umTarget { bActive ? fPixelPoint(35._fPixel, 30._fPixel) : fPP_ZERO };
        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            WrapSetScales::WriteSetScales(TraceStream(), baseWin, bActive);

        if (bAnimation)
            PushCommand(make_unique<SetScalesCmd>(&baseWin, umAnimated, umTarget));
        else
        {
            umAnimated = umTarget;
            baseWin.SendCommand(IDD_SCALES_UPDATE);
        }
    }

private:

    BaseWindow * m_pWin { nullptr };
};