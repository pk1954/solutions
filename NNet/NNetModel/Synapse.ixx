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

    Synapse(MicroMeterPnt const&);
    Synapse(Synapse const&);

    static bool TypeFits(NobType const type) { return type.IsSynapseType(); }

    void AppendMenuItems(AddMenuFunc const&) const final;

    void Dump()  const final;
    void Check() const final;

    MicroMeter    GetExtension() const final { return m_circle.GetRadius(); }
    MicroMeterPnt GetPos()       const final { return m_circle.GetPos(); }
    MicroMeterPnt GetAddPos()    const       { return m_umPntTop; }

    NobIoMode GetIoMode()    const final { return NobIoMode::internal; }
    mV        GetPotential() const final { return m_mVpotential; }

    void CollectInput() final;
    bool CompStep()     final;
    void Reconnect()    final;

    bool Includes (MicroMeterPnt const&)         const final;
    void MoveNob  (MicroMeterPnt const&)               final;
    void RotateNob(MicroMeterPnt const&, Radian const) final;
    void Link     (Nob const&, Nob2NobFunc const&)     final;
    
    void SelectAllConnected(bool const)    final;
    void PosChanged()                      final;
    void Recalc()                          final;
    void SetPosNoFix(MicroMeterPnt const&) final;

    void SetAllIncoming(PosNob&) final;
    void SetAllOutgoing(PosNob&) final;

    void AddIncoming(Pipe*) final;
    void AddOutgoing(Pipe*) final;

    void SetAddPipe(Pipe*);

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

    Pipe      * GetAddPipe()       { return m_pPipeAdd; }
    Pipe const* GetAddPipe() const { return m_pPipeAdd; }
    Pipe      * GetInPipe ()       { return m_pPipeIn; }
    Pipe const* GetInPipe () const { return m_pPipeIn; }
    Pipe      * GetOutPipe()       { return m_pPipeOut; }
    Pipe const* GetOutPipe() const { return m_pPipeOut; }

private:

    inline static MicroMeter const PIPE_HALF { PIPE_WIDTH * 0.5f };
    inline static MicroMeter const RADIUS    { PIPE_HALF };
    inline static MicroMeter const EXTENSION { PIPE_WIDTH * 0.5f };

    void drawSynapse(DrawContext const&, MicroMeter const, D2D1::ColorF const) const;
    void recalc() const;

    MicroMeterCircle m_circle;  // inpipe and outpipe meet here

    mutable MicroMeterPnt m_umPntTop    { NP_NULL }; 
    mutable MicroMeterPnt m_umPntBase1  { NP_NULL }; 
    mutable MicroMeterPnt m_umPntBase2  { NP_NULL };
    mutable MicroMeterPnt m_umPntCenter { NP_NULL };

    bool       m_bOutputBlocked { false };
    fMicroSecs m_usBlocked      { 0.0_MicroSecs };
    mV         m_mVaddInput     { 0._mV };
    Pipe *     m_pPipeIn;
    Pipe *     m_pPipeOut;
    Pipe *     m_pPipeAdd;

    bool isDefined() const { return m_pPipeIn && m_pPipeOut && m_pPipeAdd; }
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
