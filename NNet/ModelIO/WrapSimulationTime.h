// WrapSimulationTime.h 
//
// ModelIO

#pragma once

#include "NNetWrapperBase.h"

import SimulationTime;

class WrapSimulationTime : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        fMicroSecs simuTime { Cast2Float(script.ScrReadFloat()) };
        SimulationTime::Set(simuTime);
    }

    void Write(wostream & out) const final 
    {
        WriteCmdName(out);
        out << SimulationTime::Get() << endl;
    };
};
