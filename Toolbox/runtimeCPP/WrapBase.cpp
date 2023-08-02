// WrapBase.cpp
//
// Toolbox\runtimeCPP

module;

#include <string>
#include <iostream>

module WrapBase;

import Symtab;

using std::wostream;
using std::wstring;
using std::endl;


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

