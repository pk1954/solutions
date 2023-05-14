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
    NNetWrapperBase
    (
        wstring const & wstrName,
        NNetModelIO   & modelIO
    )
      : WrapBase(wstrName),
        m_modelIO(modelIO)
    {};

protected:

    NNetModelIO & m_modelIO;
};
