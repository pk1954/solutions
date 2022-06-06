// WrapProtocol.h 
//
// ModelIO

#pragma once

#include "NNetModelStorage.h"
#include "NNetWrapperBase.h"

class WrapProtocol : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        script.ScrReadString(L"version");
        double dVersion = script.ScrReadFloat();
    }

    void Write(wostream & out) const final 
    {
        out << L"Protocol version " << NNetModelStorage::PROTOCOL_VERSION << endl;
        out << endl;
    };
};
