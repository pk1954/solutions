// Fork.ixx
//
// NNetModel

module;

export module NNetModel:Fork;

import DrawContext;
import :BaseKnot;
import :NobType;
import :Pipe;
import :tHighlight;

export class Fork : public BaseKnot
{
public:

    explicit Fork(MicroMeterPnt const center)
        : BaseKnot(center, NobType::Value::fork, KNOT_WIDTH)
    {}

    Radian    GetDir()    const override { return Radian::NULL_VAL(); };
    NobIoMode GetIoMode() const override { return NobIoMode::internal; }

    //void Check() const final;

    //void Dump()  const final;

    //void Link(Nob const &, Nob2NobFunc const &) final;

	void DrawExterior(DrawContext const &, tHighlight const) const final {};
	void DrawInterior(DrawContext const &, tHighlight const) const final {};
    
    void CollectInput () final 
    {
        m_mVinputBuffer = static_cast<Pipe const&>(GetIncoming(0)).GetEndKnotPtr()->GetNextOutput();
    };

    bool CompStep() final { return false; };

private:
};