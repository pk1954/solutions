// WrapBase.cpp
//
// Toolbox\runtimeCPP

module;

#include <string>
#include <iostream>

module WrapBase;

using std::wostream;
using std::wstring;
using std::endl;

wstring const PREF_ON  { L"ON"  };
wstring const PREF_OFF { L"OFF" };

void PrefOnOff(wostream& out, bool const bOn)
{
    out << L' ' << (bOn ? PREF_ON : PREF_OFF);
}

WrapBase::WrapBase(wstring const& wstrName)
{
    SymbolTable::ScrDefConst(wstrName, this);
}

wstring const& WrapBase::GetName() const
{
    return SymbolTable::GetSymbolName(Symbol(this));
}

void WrapBase::WriteCmdName(wostream& out) const
{
    out << GetName() << L' ';
}

void WrapBaseBool::Initialize()
{
    SymbolTable::ScrDefConst(PREF_OFF, 0L);
    SymbolTable::ScrDefConst(PREF_ON, 1L);
}

WrapBaseBool::WrapBaseBool
(
    wstring const& wstrName,
    BoolType& boolType
)
  : WrapBase(wstrName),
    m_boolType(boolType)
{}

void WrapBaseBool::operator() (Script& script) const
{
    m_boolType.Set(script.ScrReadBool());
}

void WrapBaseBool::Write(wostream& out) const
{
    WriteCmdName(out);
    PrefOnOff(out, m_boolType.Get());
}

