// WrapBase.ixx
//
// Toolbox\runtimeCPP

module;

#include <string>
#include <iostream>
#include <type_traits>

export module WrapBase;

import BoolType;
import Symtab;
import Script;

using std::wostream;
using std::wstring;
using std::endl;
using std::is_base_of;
using std::remove_pointer_t;

export inline wstring const PREF_ON  { L"ON"  };
export inline wstring const PREF_OFF { L"OFF" };

export void PrefOnOff(wostream& out, bool const bOn)
{
    out << (bOn ? PREF_ON : PREF_OFF);
}

export class WrapBase : public ScriptFunctor
{
public:
    virtual ~WrapBase() = default;

    explicit WrapBase(wstring const& wstrName)
    {
        SymbolTable::ScrDefConst(wstrName, this);
    }

    virtual void Write(wostream& out) const = 0;

    wstring const& GetName() const
    {
        return SymbolTable::GetSymbolName(Symbol(this));
    }

    void WriteCmdName(wostream& out) const
    {
        out << GetName() << L' ';
    }

    void Write2(wostream& out) const
    {
        WriteCmdName(out);
        Write(out);
        out << endl;
    }
};

export class WrapBaseBool : public WrapBase
{
public:
    static void Initialize()
    {
        SymbolTable::ScrDefConst(PREF_OFF, 0L);
        SymbolTable::ScrDefConst(PREF_ON, 1L);
    }

    WrapBaseBool
    (
        wstring const& wstrName,
        BoolType& boolType
    )
        : WrapBase(wstrName),
        m_boolType(boolType)
    {}

    void operator() (Script& script) const final
    {
        m_boolType.Set(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        PrefOnOff(out, m_boolType.Get());
    }

protected:
    BoolType& m_boolType;
};

export template <typename T>
concept Wrap_t = is_base_of<WrapBase, remove_pointer_t<T>>::value;
