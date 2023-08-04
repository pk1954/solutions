// WrapSimulationTime.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapSimulationTime;

import SaveCast;
import Types;
import Signals;
import WrapBase;
import Script;

using std::endl;
using std::wostream;

export class WrapSimulationTime : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        fMicroSecs simuTime{ Cast2Float(script.ScrReadFloat()) };
        SimulationTime::Set(simuTime);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << SimulationTime::Get() << endl;
    };
};
