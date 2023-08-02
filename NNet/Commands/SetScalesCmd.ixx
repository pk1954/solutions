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
import WrapBaseBool;
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

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        BaseWindow  &baseWin,
        fPixelPoint &umAnimated,
        bool  const bOn,
        bool  const bAnimation
    )
    {
        fPixelPoint const umTarget { bOn ? fPixelPoint(35._fPixel, 30._fPixel) : fPP_ZERO };
        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            TraceStream() << NAME << SPACE << WinManager::GetWindowName(baseWin) << PrefOnOff(bOn) << endl;

        if (bAnimation)
            PushCommand(make_unique<SetScalesCmd>(&baseWin, umAnimated, umTarget));
        else
        {
            umAnimated = umTarget;
            baseWin.SendCommand(IDD_SCALES_UPDATE);
        }
    }

private:

    inline static const wstring NAME { L"SetScales" };

    BaseWindow * m_pWin { nullptr };

    inline static SetScalesFunctor m_wrapper;
};