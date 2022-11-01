// Synapse.ixx
//
// NNetModel

module;

export module NNetModel:Synapse;

import DrawContext;
import :BaseKnot;
import :NobType;
import :Pipe;
import :OutputLine;
import :tHighlight;

export class Synapse : public BaseKnot
{
public:

    explicit Synapse(MicroMeterPnt const center)
        : BaseKnot(center, NobType::Value::synapse, KNOT_WIDTH)
    {
        ReserveInputConns(2);
    }

    void Dump()  const final;
    void Check() const final;

    Radian    GetDir()    const final { return Radian::NULL_VAL(); };
    NobIoMode GetIoMode() const final { return NobIoMode::internal; }

    void CollectInput()        final;
    bool CompStep()            final;
    mV   GetNextOutput() const final;
    void Reconnect()           final;

    Pipe const& GetAddPipe () const { return GetIncoming(ADD_INDEX); }
    Pipe const& GetMainPipe() const { return GetIncoming(MAIN_INDEX); }

    void SetAddPipe (Pipe & pipe) { SetIncoming(ADD_INDEX,  &pipe); }
    void SetMainPipe(Pipe & pipe) { SetIncoming(MAIN_INDEX, &pipe); }

    void DrawExterior(DrawContext const&, tHighlight const) const final;
    void DrawInterior(DrawContext const&, tHighlight const) const final;

private:

    static const size_t MAIN_INDEX { 0 };
    static const size_t ADD_INDEX  { 1 };

    enum class tState { normal, addLineBlocked, stdInputBlocked };

    tState     m_state      { tState::normal };
    fMicroSecs m_usBlocked  { 0.0_MicroSecs };
    mV         m_mVaddInput { 0._mV };

    Pipe & getAddPipe () { return GetIncoming(ADD_INDEX); }
    Pipe & getMainPipe() { return GetIncoming(MAIN_INDEX); }
};