// Synapse.ixx
//
// NNetModel

module;

export module NNetModel:Synapse;

import DrawContext;
import :PosNob;
import :NobType;
import :Pipe;
import :OutputLine;
import :tHighlight;

export class Synapse : public PosNob
{
public:

    Synapse(Pipe * const, Pipe * const);

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

    Pipe       & GetAddPipe()             { return *m_pPipeAdd; }
    Pipe const & GetAddPipe()       const { return *m_pPipeAdd; }
    Pipe       & GetMainPipe()            { return *m_pPipeMain; }
    Pipe const & GetMainPipe()      const { return *m_pPipeMain; }
    float        GetPosOnMainPipe() const { return m_fPosOnMainPipe; }

    void SetMainPipe(Pipe* const);
    void ResetPos(MicroMeterPnt const&);

    void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
    void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

    void Apply2AllInPipes (PipeFunc const& f) const final;
    void Apply2AllOutPipes(PipeFunc const& f) const final;

    bool Apply2AllInPipesB (PipeCrit const& c) const final;
    bool Apply2AllOutPipesB(PipeCrit const& c) const final;

    void DrawExterior(DrawContext const&, tHighlight const) const final;
    void DrawInterior(DrawContext const&, tHighlight const) const final;

private:

    enum class tState { normal, addLineBlocked, stdInputBlocked };

    tState     m_state      { tState::normal };
    fMicroSecs m_usBlocked  { 0.0_MicroSecs };
    mV         m_mVaddInput { 0._mV };
    Pipe *     m_pPipeMain;
    Pipe *     m_pPipeAdd;
    float      m_fPosOnMainPipe;
};