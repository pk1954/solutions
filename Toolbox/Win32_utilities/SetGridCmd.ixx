// SetGridCmd.ixx
//
// Toolbox\Win32_utilities

export module SetGridCmd;

import std;
import IoConstants;
import Win32_Util_Resource;
import RootWindow;
import WinManager;
import WrapSetGrid;
import AnimationCommand;

using std::wostream;

using FloatAnimationCmd = AnimationCommand<float>;

export class SetGridCmd : public FloatAnimationCmd
{
public:

    SetGridCmd
    (
        RootWindow  & rootWin,
        float       & fAnimated,
        float const & fTarget
    )
      : FloatAnimationCmd(rootWin, fAnimated, fAnimated, fTarget)
    {}

    void UpdateUI() final
    {        
        FloatAnimationCmd::UpdateUI();
        SendCommand2AnimWin(IDD_GRID_UPDATE);
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
            PushCommand(make_unique<SetGridCmd>(rootWin, fAnimated, fTarget));
        }
        else
        {
            fAnimated = fTarget;
            rootWin.SendCommand(IDD_GRID_UPDATE);
        }
    }
};