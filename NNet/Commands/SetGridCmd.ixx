// SetGridCmd.ixx
//
// Commands

module;

#include <iostream>
#include "Resource.h"

export module NNetCommands:SetGridCmd;

import SaveCast;
import Win32_Util_Resource;
import WinManager;
import BaseWindow;
import :AnimationCmd;

using std::wstring;

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
            RootWinId id { WinManager::GetIdFromBaseWindow(baseWin) };
            TraceStream() << NAME << SPACE << id << SPACE << fAnimated << SPACE << fTarget << SPACE; //<< endl;
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

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            unsigned int const uiWinId  { script.ScrReadUint() };
            BaseWindow * const pBaseWin { WinManager::GetBaseWindow(RootWinId(uiWinId)) };
            bool         const bOn      { script.ScrReadBool() };
            if (pBaseWin)
                pBaseWin->SetGrid(bOn, false);
            else
            {
                //TODO: Error message
            }
        }
    } m_wrapper;
};