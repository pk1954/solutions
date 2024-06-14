// WrapSimulationTime.ixx
//
// Toolbox\Commands

module;

#include <iostream>

export module Commands:WrapSimulationTime;

import SaveCast;
import Types;
import SimulationTime;
import :Wrapper;
import Script;

using std::endl;
using std::wostream;

export class WrapSimulationTime : public Wrapper
{
public:
    using Wrapper::Wrapper;

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
