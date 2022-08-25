// WrapProtocol.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetModelStorage.h"

export module WrapProtocol;

import NNetWrapperBase;

using std::endl;
using std::wostream;

export class WrapProtocol : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        script.ScrReadString(L"version");
        double dVersion = script.ScrReadFloat();
    }

    void Write(wostream& out) const final
    {
        out << L"Protocol version " << NNetModelStorage::PROTOCOL_VERSION << endl;
        out << endl;
    };
};
