// WrapVoltage.h 
//
// ModelIO

#pragma once

#include "WrapperBase.h"

class Script;

class WrapVoltage : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {   
        setVoltage(script);
    }

private:

    void setVoltage        (Script &)              const;
    void setBaseKnotVoltage(Script &, NobId const) const;
    void setPipeVoltage    (Script &, NobId const) const;
};
