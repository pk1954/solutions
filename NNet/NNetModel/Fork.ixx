// Fork.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:Fork;

import DrawContext;
import :Nob;
import :PosNob;
import :NobType;
import :Pipe;
import :tHighlight;

export class Fork : public PosNob
{
public:

    explicit Fork(MicroMeterPnt const);

    size_t GetNrOfInConns () const final { return 1; }
    size_t GetNrOfOutConns() const final { return 2; }

    MicroMeter    GetExtension() const final { return m_circle.GetRadius(); }
    MicroMeterPnt GetPos()       const final { return m_circle.GetPos(); }
    Radian        GetDir()       const final { return Radian::NULL_VAL(); }
    NobIoMode     GetIoMode()    const final { return NobIoMode::internal; }

    Pipe* GetFirstOutgoing () { return m_pPipeOut1; }
    Pipe* GetSecondOutgoing() { return m_pPipeOut2; }
    Pipe* GetIncoming()       { return m_pPipeIn; }

    void SetPos   (MicroMeterPnt  const&)               final;
    void MoveNob  (MicroMeterPnt  const&)               final;
    void RotateNob(MicroMeterPnt  const&, Radian const) final;
    void Link     (Nob const&, Nob2NobFunc const&)      final;

    void Check() const final;

    //void Dump()  const final;

    void SetAllIncoming(PosNob&) final;
    void SetAllOutgoing(PosNob&) final;

    void Reconnect() final;

    void AddIncoming(Pipe&) final;
    void AddOutgoing(Pipe&) final;

    void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
    void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

    void Apply2AllInPipes (PipeFunc const& f) const final;
    void Apply2AllOutPipes(PipeFunc const& f) const final;

    bool Apply2AllInPipesB (PipeCrit const& c) const final;
    bool Apply2AllOutPipesB(PipeCrit const& c) const final;

    void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;
    
    void CollectInput () final { m_mVinputBuffer = m_pPipeIn->GetNextOutput(); };

    bool CompStep() final { return false; };

private:
    MicroMeterCircle m_circle;
    
    Pipe * m_pPipeIn   { nullptr };
    Pipe * m_pPipeOut1 { nullptr };
    Pipe * m_pPipeOut2 { nullptr };
};

export Fork const* Cast2Fork(Nob const* pNob)
{
    assert(pNob->IsFork());
    return static_cast<Fork const*>(pNob);
}

export Fork* Cast2Fork(Nob* pNob)
{
    assert(pNob->IsFork());
    return static_cast<Fork*>(pNob);
}
