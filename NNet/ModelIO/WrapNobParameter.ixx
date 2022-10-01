// WrapNobParameter.ixx
//
// ModelIO

module;

#include <compare>
#include <cassert>

export module WrapNobParameter;

import SaveCast;
import NNetWrapperBase;
import Script;
import NNetModel;

export class WrapNobParameter : public NNetWrapperBase  // Legacy
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        script.ScrReadString(L"InputNeuron");
        NobId const id(script.ScrReadLong());
        auto  const param(static_cast<ParamType::Value>(script.ScrReadUint()));
        assert(param == ParamType::Value::pulseRate);
        script.ScrReadSpecial(L'=');
        Cast2Float(script.ScrReadFloat()); // 
    }
};
