// NNetWrapperBase.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module NNetWrapperBase;

import WrapBase;
import NNetModelIO;

using std::wstring;
using std::wostream;

export class NNetWrapperBase : public WrapBase
{
public:
    explicit NNetWrapperBase
    (
        wstring const & wstrName,
        NNetModelIO   & modelIO
    )
      : WrapBase(wstrName),
        m_modelIO(modelIO)
    {};

    void Write(wostream & out) const override { /* for legacy commands */ };

protected:

    NNetModelIO & m_modelIO;
};
