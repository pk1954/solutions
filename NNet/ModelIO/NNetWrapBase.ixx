// NNetWrapBase.ixx
//
// ModelIO

module;

#include <string>

export module NNetWrapBase;

import WrapBase;
import NNetPreferences;

using std::wstring;

export class NNetWrapBase : public WrapBase
{
public:
    NNetWrapBase
    (
        wstring const& wstrName,
        NNetPreferences& pref
    )
        : WrapBase(wstrName),
        m_pref(pref)
    {}

protected:
    NNetPreferences& m_pref;
};
