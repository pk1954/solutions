// Synapse.ixx
//
// NNetModel

module;

#include <cassert>
#include <cmath>

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

    MicroMeter GetExtension() const final { return NEURON_RADIUS; }

    Radian    GetDir()    const final { return Radian::NULL_VAL(); };
    NobIoMode GetIoMode() const final { return NobIoMode::internal; }

    void CollectInput()        final;
    bool CompStep()            final;
    mV   GetNextOutput() const final;
    void Reconnect()           final;

    MicroMeterPnt GetPos   ()                              const final;
    bool          Includes (MicroMeterPnt  const&)         const final;
    void          SetPos   (MicroMeterPnt  const&)               final;
    void          MoveNob  (MicroMeterPnt  const&)               final;
    void          RotateNob(MicroMeterPnt  const&, Radian const) final;
    void          Link     (Nob const&, Nob2NobFunc const&)      final;

    void SetAllIncoming(PosNob&) final;
    void SetAllOutgoing(PosNob&) final;

    void RemoveFromMainPipe() { m_pPipeMain->RemoveSynapse(this); }
    void Add2MainPipe()       { m_pPipeMain->AddSynapse(this); }

    void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
    void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

    void Apply2AllInPipes (PipeFunc const& f) const final;
    void Apply2AllOutPipes(PipeFunc const& f) const final;

    bool Apply2AllInPipesB (PipeCrit const& c) const final;
    bool Apply2AllOutPipesB(PipeCrit const& c) const final;

    void DrawExterior(DrawContext const&, tHighlight const) const final;
    void DrawInterior(DrawContext const&, tHighlight const) const final;

    size_t GetNrOfInConns () const final { return 2; }
    size_t GetNrOfOutConns() const final { return 1; }

    Pipe      * GetAddPipe()             { return m_pPipeAdd; }
    Pipe const* GetAddPipe()       const { return m_pPipeAdd; }
    Pipe      * GetMainPipe()            { return m_pPipeMain; }
    Pipe const* GetMainPipe()      const { return m_pPipeMain; }
    float       GetPosOnMainPipe() const { return m_fPosOnMainPipe; }

    void SetMainPipe(Pipe* const);

private:

    inline static MicroMeter const PIPE_HALF { PIPE_WIDTH * 0.5f };
    inline static MicroMeter const RADIUS    { PIPE_HALF };
    inline static MicroMeter const EXTENSION { PIPE_WIDTH * 0.7f };
    inline static float      const SQRT3     { sqrtf(3.0f) };
    inline static float      const SQRT3DIV3 { SQRT3 / 3.0f };

    void calcPos() const;
    void resetPos(MicroMeterPnt const&);
    void drawSynapse(DrawContext const&, MicroMeter const, MicroMeter const, D2D1::ColorF const) const;

    enum class tState { normal, addLineBlocked, stdInputBlocked };

    mutable float         m_fDirection      { 1.0f };
    mutable MicroMeterPnt m_umPntPipeAnchor { NP_NULL };
    mutable MicroMeterPnt m_umPntCenter     { NP_NULL };

    tState        m_state          { tState::normal };
    fMicroSecs    m_usBlocked      { 0.0_MicroSecs };
    mV            m_mVaddInput     { 0._mV };
    Pipe *        m_pPipeMain;
    Pipe *        m_pPipeAdd;
    float         m_fPosOnMainPipe;
};

export Synapse const* Cast2Synapse(Nob const*);
export Synapse      * Cast2Synapse(Nob*);
