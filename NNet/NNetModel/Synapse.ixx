// Synapse.ixx
//
// NNetModel

module;

export module NNetModel:Synapse;

import DrawContext;
import :Knot;
import :NobType;
import :Pipe;
import :OutputLine;
import :tHighlight;

export class Synapse : public Knot
{
public:

    explicit Synapse(MicroMeterPnt const center)
        : Knot(center, NobType::Value::synapse)
    {}

    void Check() const final;

    void CollectInput()        final;
    bool CompStep()            final;
    mV   GetNextOutput() const final;

    Pipe const & GetAddPipe() const { return GetIncoming(1); }

    void DrawExterior(DrawContext const&, tHighlight const) const final;
    void DrawInterior(DrawContext const&, tHighlight const) const final;

private:

    enum class tState { normal, addLineBlocked, stdInputBlocked };

    tState     m_state      { tState::normal };
    fMicroSecs m_usBlocked  { 0.0_MicroSecs };
    mV         m_mVaddInput { 0._mV };
};