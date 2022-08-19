// WrapBase.ixx
//
// Toolbox\runtimeCPP

module;

#include <string>
#include <ostream>
#include "Script.h"

export module WrapBase;

import Symtab;

using std::wostream;
using std::wstring;
using std::endl;

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
        out << GetName() << L" ";
    }

    void Write2(wostream& out) const
    {
        WriteCmdName(out);
        Write(out);
        out << endl;
    }
};