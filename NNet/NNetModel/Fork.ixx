// Fork.ixx
//
// NNetModel

module;

export module NNetModel:Fork;

import DrawContext;
import :Knot;
import :NobType;
import :Pipe;
import :tHighlight;

export class Fork : public Knot
{
public:

    explicit Fork(MicroMeterPnt const center)
        : Knot(center, NobType::Value::fork)
    {}

    //void Check() const final;

    //void Dump()  const final;

    //void Link(Nob const &, Nob2NobFunc const &) final;

    //void CollectInput () final;
    //bool CompStep() final;

private:
};