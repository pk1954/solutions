// Synapse.ixx
//
// NNetModel

module;

export module NNetModel:Synapse;

import DrawContext;
import :Nob;
import :NobType;
import :Pipe;
import :OutputLine;
import :tHighlight;

export class Synapse : public Nob
{
public:

    Synapse(Pipe&, Pipe&);

    void Dump()  const final;
    void Check() const final;

    MicroMeter GetExtension() const { return NEURON_RADIUS; }

    Radian    GetDir()    const final { return Radian::NULL_VAL(); };
    NobIoMode GetIoMode() const final { return NobIoMode::internal; }

    void CollectInput()        final;
    bool CompStep()            final;
    mV   GetNextOutput() const final;
    void Reconnect()           final;

    MicroMeterPnt GetPos()                                    const final;
    void          Expand      (MicroMeterRect&)               const final;
    bool          IsIncludedIn(MicroMeterRect const&)         const final;
    bool          Includes    (MicroMeterPnt  const&)         const final;
    void          SetPos      (MicroMeterPnt  const&)               final;
    void          MoveNob     (MicroMeterPnt  const&)               final;
    void          RotateNob   (MicroMeterPnt  const&, Radian const) final;
    void          Link        (Nob const&, Nob2NobFunc const&)      final;

    Pipe const& GetAddPipe () const { return m_pipeAdd; }
    Pipe const& GetMainPipe() const { return m_pipeMain; }

    void DrawExterior(DrawContext const&, tHighlight const) const final;
    void DrawInterior(DrawContext const&, tHighlight const) const final;

private:

    enum class tState { normal, addLineBlocked, stdInputBlocked };

    tState     m_state      { tState::normal };
    fMicroSecs m_usBlocked  { 0.0_MicroSecs };
    mV         m_mVaddInput { 0._mV };
    Pipe&      m_pipeMain;
    Pipe&      m_pipeAdd;
    float      m_fPosOnMainPipe;
};