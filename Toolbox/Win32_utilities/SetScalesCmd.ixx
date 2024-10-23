// SetScalesCmd.ixx
//
// Toolbox\Win32_utilities

export module SetScalesCmd;

import std;
import Types;
import IoConstants;
import Win32_Util_Resource;
import RootWindow;
import WinManager;
import AnimationCommand;
import WrapSetScales;
import Scale;

using fPixelPointAnimationCmd = AnimationCommand<fPixelPoint>;

export class SetScalesCmd : public fPixelPointAnimationCmd
{
public:
    SetScalesCmd
    (
        RootWindow       & rootWin,
        fPixelPoint      & fAnimated,
        fPixelPoint const& fTarget
    )
      : fPixelPointAnimationCmd(rootWin, fAnimated, fAnimated, fTarget)
    {}

    void UpdateUI() final
    {
        fPixelPointAnimationCmd::UpdateUI();
        SendCommand2AnimWin(IDD_SCALES_UPDATE);
    }

    static void Push
    (
        RootWindow  &rootWin,
        fPixelPoint &umAnimated,
        bool  const  bActive,
        bool  const  bAnimation
    )
    {
        fPixelPoint const umTarget 
        { 
            bActive 
            ? Convert2fPixelPoint(PixelPoint(V_SCALE_WIDTH, H_SCALE_HEIGHT))
            : fPP_ZERO 
        };

        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            WrapSetScales::WriteSetScales(TraceStream(), rootWin, bActive);

        if (bAnimation)
            PushCommand(make_unique<SetScalesCmd>(rootWin, umAnimated, umTarget));
        else
        {
            umAnimated = umTarget;
            rootWin.SendCommand(IDD_SCALES_UPDATE);
        }
    }
};