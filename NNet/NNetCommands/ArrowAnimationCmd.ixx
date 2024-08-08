// ArrowAnimationCmd.ixx
//
// NNetCommands

export module ArrowAnimationCmd;

import std;
import Types;
import IoUtil;
import IoConstants;
import WinManager;
import Win32_Util_Resource;
import AnimationCommand;

using MicroMeterAnimationCmd = AnimationCommand<MicroMeter>;

export class ArrowAnimationCmd : public MicroMeterAnimationCmd
{
public:
    using MicroMeterAnimationCmd::MicroMeterAnimationCmd;

    void UpdateUI() final
    {
        MicroMeterAnimationCmd::UpdateUI();
        m_rootWinAnim.Notify(true);
    };

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        MicroMeter & umAnimated,
        bool const   bOn,
        bool const   bAnimation
    )
    {
        MicroMeter const umTarget { bOn ? 30.0_MicroMeter : 0._MicroMeter };

        if (umTarget == umAnimated)
            return;

        if (IsTraceOn())
            TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        if (bAnimation)
            PushCommand
            (
                make_unique<ArrowAnimationCmd>
                (
                    *WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW)),
                    umAnimated, 
                    umAnimated, 
                    umTarget
                )
            );
        else
            umAnimated = umTarget;
    }

private:

    inline static const wstring NAME { L"ArrowAnimation" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            MicroMeter animated { ScrReadMicroMeter(script) };
            bool       bOn      { script.ScrReadBool() };
            ArrowAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper { NAME };
};