// WrapProtocol.ixx
//
// ModelIO

module;

#include <iostream>
#include <iomanip>

export module WrapProtocol;

import NNetWrapperBase;
import NNetModelStorage;

using std::endl;
using std::wostream;
using std::setprecision;

export class WrapProtocol : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        script.ScrReadString(L"version");
        double dVersion = script.ScrReadFloat();
        if (dVersion < NNetModelStorage::PROTOCOL_VERSION)
            throw ProtocollException(dVersion);
    }

    void Write(wostream& out) const final
    {
        out << L"Protocol version " << setprecision(2) << NNetModelStorage::PROTOCOL_VERSION << endl;
        out << endl;
    };
};
