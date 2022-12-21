// Synapse.ixx
//
// NNetModel

module;

#include <cassert>
#include <cmath>

export module NNetModel:Synapse;

import DrawContext;
import :NobId;
import :PosNob;
import :NobType;
import :Pipe;
import :OutputLine;
import :tHighlight;

export class Synapse : public PosNob
{
public:

    Synapse(Pipe* const, Pipe* const);
    Synapse(Nob * const, Nob * const);
    Synapse(Synapse const&);

    static bool TypeFits(NobType const type) { return type.IsSynapseType(); }

    void AppendMenuItems(AddMenuFunc const&) const final;

    void Dump()  const final;
    void Check() const final;

    MicroMeter GetExtension() const final { return NEURON_RADIUS; }

    Radian    GetDir()    const final { return Radian::NULL_VAL(); };
    NobIoMode GetIoMode() const final { return NobIoMode::internal; }

    void CollectInput() final;
    bool CompStep()     final;
    void Reconnect()    final;

    MicroMeterPnt GetPos   ()                              const final;
    bool          Includes (MicroMeterPnt  const&)         const final;
    void          MoveNob  (MicroMeterPnt  const&)               final;
    void          RotateNob(MicroMeterPnt  const&, Radian const) final;
    void          Link     (Nob const&, Nob2NobFunc const&)      final;
    void SelectAllConnected(bool const bFirst) final
    {
        if (!IsSelected() || bFirst)
        {
            Nob::Select(true);
            m_pPipeMain->SelectAllConnected(false);
            m_pPipeAdd ->SelectAllConnected(false);
        }
    }

    void SetPos(MicroMeterPnt  const&) final { assert(false); }

    void SetAllIncoming(PosNob&) final;
    void SetAllOutgoing(PosNob&) final;

    void RemoveFromMainPipe();
    void Add2MainPipe();

    void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
    void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

    void Apply2AllInPipes (PipeFunc const&) final;
    void Apply2AllOutPipes(PipeFunc const&) final;

    void Apply2AllInPipesC (PipeFuncC const&) const final;
    void Apply2AllOutPipesC(PipeFuncC const&) const final;

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

    void SetPosOnMainPipe(float const);
    void SetAddPipe    (Pipe* const);
    void SetMainPipe   (Pipe* const);
    void ChangeMainPipe(Pipe* const);

    void RecalcAll(MicroMeterPnt const&);
    void RecalcPositions();

private:

    inline static MicroMeter const PIPE_HALF { PIPE_WIDTH * 0.5f };
    inline static MicroMeter const RADIUS    { PIPE_HALF };
    inline static MicroMeter const EXTENSION { PIPE_WIDTH * 0.7f };
    inline static float      const SQRT3     { sqrtf(3.0f) };
    inline static float      const SQRT3DIV3 { SQRT3 / 3.0f };

    void drawSynapse(DrawContext const&, MicroMeter const, MicroMeter const, D2D1::ColorF const) const;

    float         m_fDirection      { 1.0f };
    MicroMeterPnt m_umPntPipeAnchor { NP_NULL };
    MicroMeterPnt m_umPntCenter     { NP_NULL };

    bool       m_bOutputBlocked { false };
    fMicroSecs m_usBlocked      { 0.0_MicroSecs };
    mV         m_mVaddInput     { 0._mV };
    Pipe *     m_pPipeMain;
    Pipe *     m_pPipeAdd;
    float      m_fPosOnMainPipe;
};

export Synapse const* Cast2Synapse(Nob const* pNob)
{
    assert(pNob);
    assert(pNob->IsSynapse());
    return static_cast<Synapse const*>(pNob);
}

export Synapse* Cast2Synapse(Nob* pNob)
{
    assert(pNob);
    assert(pNob->IsSynapse());
    return static_cast<Synapse*>(pNob);
}
