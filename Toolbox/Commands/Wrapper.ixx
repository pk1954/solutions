// Wrapper.ixx
//
// Toolbox\Commands

export module Commands:Wrapper;

import std;
export import IoConstants;
export import BoolType;
export import Script;
export import Symtab;

export using std::wostream;
export using std::wstring;

using std::is_base_of;
using std::remove_pointer_t;

export class Wrapper : public ScriptFunctor
{
public:
    explicit Wrapper(wstring const& wstrName)
    {
        SymbolTable::ScrDefConst(wstrName, this);
    }

    virtual ~Wrapper() = default;

    virtual void Write(wostream& out) const {};

    wstring const& GetName() const
    {
        return SymbolTable::GetSymbolName(Symbol(this));
    }

    void WriteCmdName(wostream& out) const
    {
        out << GetName() << SPACE;
    }
};

export template <typename T>
concept Wrap_t = is_base_of<Wrapper, remove_pointer_t<T>>::value;
