// WrapSimulationTime.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetWrapperBase.h"
#include "Script.h"

export module WrapSimulationTime;

import SaveCast;
import SimulationTime;

using std::endl;

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
