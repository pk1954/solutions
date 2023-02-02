// MicroSensor.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetModelWriterInterface;
import :SignalSource;
import :NobId;
import :Nob;

using std::wostream;

export class MicroSensor : public SignalSource
{
public:

    MicroSensor(NobId const, NNetModelWriterInterface&);

    virtual ~MicroSensor() = default;

    void Dump()                               const final;
    mV   GetSignalValue()                     const final;
    void WriteInfo(wostream&)                 const final;
    void Draw(DrawContext const&, bool const) const final;

private:

    Nob* m_pNob;
};