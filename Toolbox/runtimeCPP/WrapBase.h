// WrapBase.h
//
// Toolbox\runtimeCPP

#pragma once

#include <string>
#include <vector>
#include "symtab.h"

using std::wostream;
using std::wstring;
using std::vector;
using std::endl;

class WrapBase : public ScriptFunctor
{
public:
    explicit WrapBase(wstring const & wstrName)
    {
        SymbolTable::ScrDefConst(wstrName, this);
    }

    virtual void Write(wostream& out) const = 0;

    void Write2(wostream & out) const
    {
        out << SymbolTable::GetSymbolName(Symbol(this)) << L" ";
        Write(out);
        out << endl;
    }
};