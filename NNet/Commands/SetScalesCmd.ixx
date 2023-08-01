// SetScalesCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:SetScalesCmd;

import Win32_Util_Resource;
import BaseWindow;
import WinManager;
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
        {
            RootWinId id { WinManager::GetIdFromBaseWindow(baseWin) };
            TraceStream() << NAME << SPACE << id << SPACE << bOn << endl;
        }

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

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            unsigned int const uiWinId  { script.ScrReadUint() };
            bool         const bActive  { script.ScrReadBool() };
            BaseWindow * const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
            if (pBaseWin)
                pBaseWin->SetScales(bActive, false);
            else
            {
                //TODO: Error message
            }
        }
    } m_wrapper;
};