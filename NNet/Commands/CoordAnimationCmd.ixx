// CoordAnimationCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:CoordAnimationCmd;

import :AnimationCmd;

using U2DAnimationCmd = AnimationCmd<Uniform2D<MicroMeter>>;

export class CoordAnimationCmd : public U2DAnimationCmd
{
public:

    static void Initialize(Observable * const pObservable)
    {
        m_pCoordObservable = pObservable;
    }

    using U2DAnimationCmd::U2DAnimationCmd;

    void UpdateUI() final
    {
        U2DAnimationCmd::UpdateUI();
        Command::UpdateUI();
        if (m_pCoordObservable)
            m_pCoordObservable->NotifyAll(false);
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        Uniform2D<MicroMeter>      & animated,
        Uniform2D<MicroMeter> const& target
    )
    {
        if (IsTraceOn())
            TraceStream() << NAME << endl;
        PushCommand(make_unique<CoordAnimationCmd>(animated, target));
    }

private:

    inline static const wstring NAME { L"CoordAnimation" };

    inline static Observable* m_pCoordObservable { nullptr };

    inline static struct Wrapper : public WrapBase
    {
        using WrapBase::WrapBase;
        void operator() (Script& script) const final
        {}
    } m_wrapper { NAME };
};