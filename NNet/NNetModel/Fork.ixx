// Fork.ixx
//
// NNetModel

module;

export module NNetModel:Fork;

import DrawContext;
import :PosNob;
import :NobType;
import :Pipe;
import :tHighlight;

export class Fork : public PosNob
{
public:

    explicit Fork(MicroMeterPnt const center)
      : PosNob(NobType::Value::fork),
        m_circle(center, KNOT_WIDTH)
    {}

    MicroMeter    GetExtension() const       { return m_circle.GetRadius(); }
    MicroMeterPnt GetPos()       const final { return m_circle.GetPos(); }
    Radian        GetDir()       const final { return Radian::NULL_VAL(); }
    NobIoMode     GetIoMode()    const final { return NobIoMode::internal; }

    void SetPos   (MicroMeterPnt  const&)               final;
    void MoveNob  (MicroMeterPnt  const&)               final;
    void RotateNob(MicroMeterPnt  const&, Radian const) final;
    void Link     (Nob const&, Nob2NobFunc const&)      final;

    void Check() const final;

    //void Dump()  const final;

    void Expand      (MicroMeterRect&)       const final;
    bool IsIncludedIn(MicroMeterRect const&) const final;
    bool Includes    (MicroMeterPnt  const&) const final;

    void Reconnect() final 
    { 
        m_pPipeIn  ->SetEndPnt(this);
        m_pPipeOut1->SetStartPnt(this);
        m_pPipeOut2->SetStartPnt(this);
    };

    void SetIncoming(Pipe * pPipeIn) 
    { 
        m_pPipeIn = pPipeIn; 
    }

    void SetOutgoing(Pipe * pPipeOut1, Pipe * pPipeOut2) 
    { 
        m_pPipeOut1 = pPipeOut1;
        m_pPipeOut2 = pPipeOut2;
    }

    void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
    void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

    void Apply2AllInPipes (PipeFunc const& f) const final;
    void Apply2AllOutPipes(PipeFunc const& f) const final;

    bool Apply2AllInPipesB (PipeCrit const& c) const final;
    bool Apply2AllOutPipesB(PipeCrit const& c) const final;

    void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;
    
    void CollectInput () final 
    {
        m_mVinputBuffer = m_pPipeIn->GetEndNobPtr()->GetNextOutput();
    };

    bool CompStep() final { return false; };

private:
    MicroMeterCircle m_circle;
    
    Pipe * m_pPipeIn   { nullptr };
    Pipe * m_pPipeOut1 { nullptr };
    Pipe * m_pPipeOut2 { nullptr };
};