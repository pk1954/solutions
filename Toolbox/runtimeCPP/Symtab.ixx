// Symtab.ixx                                    *
//                                                                          *
// Toolbox\runtimeCPP

module;

#include <map>
#include <cassert>
#include <string>
#include "Script.h"

export module Symtab;

using std::map;
using std::wstring;

export enum class tSTYPE   // Type of symbolic value 
{ 
    UnknownSTYPE,   // symbol is not defined  
    Function,       // name of a function     
    ULongConst,     // unsigned long integer  
    LongConst,      // long integer           
    FloatConst,     // float value            
    StringConst     // string value           
}; 

export class Symbol
{
public:

    Symbol() = default;

    explicit Symbol(ScriptFunctor const * const pFunc) : m_type(tSTYPE::Function   ), m_pFunc    (pFunc) { };
    explicit Symbol(long                  const l    ) : m_type(tSTYPE::LongConst  ), m_lValue   (l)     { };
    explicit Symbol(unsigned long         const ul   ) : m_type(tSTYPE::ULongConst ), m_ulValue  (ul)    { };
    explicit Symbol(wstring       const &       wstr ) : m_type(tSTYPE::StringConst), m_wstrValue(wstr)  { };
    explicit Symbol(double                const d    ) : m_type(tSTYPE::FloatConst ), m_dValue   (d)     { };

    bool operator< (const Symbol &) const;

    tSTYPE                GetSymbolType () const { return m_type; };
    double                GetFloatConst () const { assert(m_type == tSTYPE::FloatConst ); return   m_dValue;    };
    ScriptFunctor const & GetFunction   () const { assert(m_type == tSTYPE::Function   ); return * m_pFunc;     };
    wstring       const & GetStringConst() const { assert(m_type == tSTYPE::StringConst); return   m_wstrValue; };
    long                  GetLongConst  () const { assert(m_type == tSTYPE::LongConst  ); return   m_lValue;    };
    unsigned long         GetUlongConst () const { assert(m_type == tSTYPE::ULongConst ); return   m_ulValue;   };

private:
    tSTYPE                m_type      { tSTYPE::UnknownSTYPE };
    ScriptFunctor const * m_pFunc     { nullptr };  // type: Function    
    long                  m_lValue    { 0L };       // type: LogConst    
    unsigned long         m_ulValue   { 0L };       // type: ULongConst  
    wstring               m_wstrValue { L"" };      // type: StringConst 
    double                m_dValue    { 0.0f };     // type: FloatConst  
};

export class SymbolTable
{
public:

    static void ScrDefConst(wstring const &, ScriptFunctor const * const);
    static void ScrDefConst(wstring const &, wstring const &    );
    static void ScrDefConst(wstring const &, long const         );
    static void ScrDefConst(wstring const &, unsigned long const);
    static void ScrDefConst(wstring const &, double const       );

    static Symbol  const & GetSymbolFromName(wstring const &);
    static wstring const & GetSymbolName    (Symbol  const &);

    static void Clear();

private:
    static void addSymbol(wstring const &, Symbol const &);

    inline static map <wstring, Symbol, std::less<>> m_SymbolTab  {};
    inline static map <Symbol,  wstring>             m_ReverseTab {};
};
