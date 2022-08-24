// WrapProtocol.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetModelStorage.h"
#include "NNetWrapperBase.h"

export module WrapProtocol;

using std::endl;

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
