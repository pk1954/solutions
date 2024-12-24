// BoolWrapper.ixx
//
// Toolbox\Commands

export module Commands:BoolWrapper;

export import BoolType;
export import :Wrapper;

import std;
import IoConstants;
import RunTime;

export inline wstring const PREF_ON  { L"ON" };
export inline wstring const PREF_OFF { L"OFF" };

export wstring PrefOnOff(bool const bOn)
{
    return SPACE + (bOn ? PREF_ON : PREF_OFF);
}

export void PrefOnOff(wostream& out, bool const bOn)
{
    out << PrefOnOff(bOn);
}

export class BoolWrapper : public Wrapper
{
public:
    static void Initialize()
    {
        SymbolTable::ScrDefConst(PREF_OFF, 0L);
        SymbolTable::ScrDefConst(PREF_ON, 1L);
    }

    BoolWrapper
    (
        wstring const& wstrName,
        BoolType& boolType
    )
      : Wrapper(wstrName),
        m_boolType(boolType)
    {}

    void operator() (Script& script) const
    {
        m_boolType.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const
    {
        WriteCmdName(out);
        PrefOnOff(out, m_boolType.Get());
    }

protected:
    BoolType& m_boolType;
};
