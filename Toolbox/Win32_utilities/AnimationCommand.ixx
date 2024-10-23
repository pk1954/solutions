// AnimationCommand.ixx
//
// Toolbox\Win32_utilities

export module AnimationCommand;

import std;
import RootWindow;
import Animation;
import WinCommand;
import Win32_Util_Resource;

export import Commands;

using std::unique_ptr;
using std::bit_cast;

export using std::wstring;
export using std::make_unique;

export template <typename ANIM_TYPE>
class AnimationCommand : public WinCommand
{
public:
    AnimationCommand
    (
        RootWindow     & rootWinAnim,
        ANIM_TYPE      & animated,
        ANIM_TYPE const& start,
        ANIM_TYPE const& target
    )
      : m_rootWinAnim(rootWinAnim),
        m_animated(animated),
        m_start (start),
        m_target(target)
    {
        m_upAnimation = make_unique<Animation<ANIM_TYPE>>
        (
            [this](bool const bTargetReached)
            {
                m_rootWinAnim.PostMsg  // calls AnimationUpdate from UI thread
                (
                    WM_APP_UI_CALL, 
                    static_cast<WPARAM>(bTargetReached),
                    bit_cast<LPARAM>(this)
                );
            }
        );
    }

    void SetNrOfSteps(unsigned int const  uiNrOfSteps)
    {
        m_upAnimation->SetNrOfSteps(uiNrOfSteps);
    }

    virtual void UpdateUI() { m_upAnimation->Update(); }

    void Do  () final { m_upAnimation->Start(&m_animated, m_start,  m_target); }
    void Undo() final { m_upAnimation->Start(&m_animated, m_target, m_start ); }

    bool IsAsyncCommand() final { return true; };
    bool TargetReached() { return m_upAnimation->TargetReached(); }

    void NotifyAnimWin(bool const bImmediate) { m_rootWinAnim.Notify(bImmediate); }

    void PostCommand2AnimWin(WPARAM const wParam, LPARAM const lParam = 0)
    {
        m_rootWinAnim.PostCommand(wParam, lParam);
    }

    void SendCommand2AnimWin(WPARAM const wParam, LPARAM const lParam = 0)
    {
        m_rootWinAnim.SendCommand(wParam, lParam);
    }

private:

    RootWindow    & m_rootWinAnim;
    ANIM_TYPE     & m_animated;
    ANIM_TYPE const m_start;
    ANIM_TYPE const m_target;

    unique_ptr<Animation<ANIM_TYPE>> m_upAnimation;
};