// SetGridCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:SetGridCmd;

import IoConstants;
import Win32_Util_Resource;
import BaseWindow;
import WinManager;
import WrapBaseBool;
import WrapSetGrid;
import :AnimationCmd;

using std::wstring;
using std::wostream;

using FloatAnimationCmd = AnimationCmd<float>;

export class SetGridCmd : public FloatAnimationCmd
{
public:

    SetGridCmd
    (
        BaseWindow * const pWin,
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

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        BaseWindow &baseWin,
        float      &fAnimated,
        bool  const bActive,
        bool  const bAnimation
    )
    {
        float const fTarget { bActive ? 1.0f : 0.0f };
        if (fTarget == fAnimated)
            return;

        if (IsTraceOn())
        {
            TraceStream() << NAME << SPACE << WinManager::GetWindowName(baseWin) << PrefOnOff(bActive) << SPACE;
//            TraceStream() << std::endl;
        }

        if (bAnimation)
        {
            PushCommand(make_unique<SetGridCmd>(&baseWin, fAnimated, fTarget));
        }
        else
        {
            fAnimated = fTarget;
            baseWin.SendCommand(IDD_GRID_UPDATE);
        }
    }

private:

    inline static const wstring NAME { L"SetGrid" };

    BaseWindow * m_pWin { nullptr };

    inline static SetGridFunctor m_wrapper;
};