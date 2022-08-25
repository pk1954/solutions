// WrapSimulationTime.ixx
//
// ModelIO

module;

#include <iostream>
#include "Script.h"

export module WrapSimulationTime;

import SaveCast;
import BasicTypes;
import SimulationTime;
import NNetWrapperBase;

using std::endl;
using std::wostream;

export class WrapSimulationTime : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

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
