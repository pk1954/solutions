// NextMoveCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextMoveCmd;

import Types;
import AnimationCommand;

using PosDirAnimationCmd = AnimationCommand<PosDir>;

export class NextMoveCmd : public PosDirAnimationCmd
{
public:

    NextMoveCmd
    (
        RootWindow   & rootWin,
        PosDir       & posDirAnimated,
        PosDir const & posDirTarget
    )
        : PosDirAnimationCmd(rootWin, posDirAnimated, posDirAnimated, posDirTarget)
    {}

    void UpdateUI() final
    {
        PosDirAnimationCmd::UpdateUI();
        m_rootWinAnim.Notify(true);
    };

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        PosDir       &posDirAnimated,
        PosDir const &posDirTarget,
        bool   const  bAnimation
    )
    {
        if (posDirTarget == posDirAnimated)
            return;

        //if (IsTraceOn())
        //    TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        //if (bAnimation)
        //    PushCommand
        //    (
        //        make_unique<ArrowAnimationCmd>
        //        (
        //            *WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW)),
        //            umAnimated, 
        //            umAnimated, 
        //            umTarget
        //        )
        //    );
        //else
        //    umAnimated = umTarget;
    }

private:
    inline static const wstring NAME { L"NextMoveCmd" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            //MicroMeter animated { ScrReadMicroMeter(script) };
            //bool       bOn      { script.ScrReadBool() };
            //ArrowAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper { NAME };
};