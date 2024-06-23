// DeletePanelCmd.ixx
//
// NNetViewer

module;

#include <Resource.h>

export module DeletePanelCmd;

import RootWindow;
import AnimationCommand;
import PanelPlatform;

using PanelRectsAnimationCmd = AnimationCommand<PANEL_RECTS>;

export class DeletePanelCmd : public PanelRectsAnimationCmd
{
public:
    DeletePanelCmd
    (
        RootWindow * const pWin,
        PANEL_RECTS      & animated,
        PANEL_RECTS const& start,
        PANEL_RECTS const& target
    )
      : PanelRectsAnimationCmd(animated, start, target),
        m_pWin(pWin)
    {
      SetNrOfSteps(35);
    }

    void UpdateUI() final
    {
        PanelRectsAnimationCmd::UpdateUI();
        m_pWin->SendCommand(IDD_ANIMATE_PANELS);
    }

    static void Push
    (
        RootWindow       & rootWin,
        PANEL_RECTS      & animated,
        PANEL_RECTS const& start,
        PANEL_RECTS const& target
    )
    {
        PushCommand(make_unique<DeletePanelCmd>(&rootWin, animated, start, target));
    }

private:

    RootWindow * m_pWin { nullptr };
};