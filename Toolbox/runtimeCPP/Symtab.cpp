// Symtab.cpp                                                    *
//                                                                          *
// Toolbox\runtimeCPP

module RunTime:Symtab;

import std;
import :Script;
import IoConstants;

using std::endl;
using std::map;
using std::wstring;
using std::wostream;
using std::unique_ptr;
using std::make_unique;
using std::setprecision;

void SymbolTable::Dump(wostream& out)
{
    out << setprecision(20);
    Apply2All
    (
        [&out](auto const i)
        {
            out << i.first << SPACE << i.second << endl;
        }
    );
}

bool Symbol::operator< (const Symbol & rhs) const
{
    if (m_type < rhs.m_type) 
        return true;
    
    switch (m_type)
    {
    case tSTYPE::Function:            // name of a function     
        return m_pFunc < rhs.m_pFunc;

    case tSTYPE::ULongConst:          // unsigned long integer  
        return m_ulValue < rhs.m_ulValue;

    case tSTYPE::LongConst:          // long integer           
        return m_lValue < rhs.m_lValue;

    case tSTYPE::FloatConst:          // float value            
        return m_dValue < rhs.m_dValue;

    case tSTYPE::StringConst:          // string value           
        return m_wstrValue < rhs.m_wstrValue;

    default:
          ;// assert(false);
    }
    return false;
}

wostream& operator<< (wostream& out, Symbol const& symbol)
{
    switch (symbol.GetSymbolType())
    {
    case tSTYPE::UnknownSTYPE:
        //assert(false);
        break;

    case tSTYPE::Function:
        out << L"Function";
        break;

    case tSTYPE::ULongConst:
        out << L"ULongConst  " << symbol.GetUlongConst();
        break;

    case tSTYPE::LongConst:
        out << L"LongConst   " << symbol.GetLongConst();
        break;

    case tSTYPE::FloatConst:
        out << L"FloatConst  " << symbol.GetFloatConst();
        break;

    case tSTYPE::StringConst:
        out << L"StringConst " << Quoted(symbol.GetStringConst());
        break;

    default:
        ;  //assert(false);
    }
    return out;
}

// GetSymbolFromName: Find symbol in symbol table with error handling

Symbol const & SymbolTable::GetSymbolFromName(wstring const & wstrKey)
{
    auto const iter { m_upSymbolTab->find(wstrKey) };

    if (iter == m_upSymbolTab->end())
        ScriptErrorHandler::symbolError(wstrKey);

    return iter->second;
}

wstring const & SymbolTable::GetSymbolName(Symbol const & symbol)
{
    auto const iter { m_upReverseTab->find(symbol) };

    //assert(iter != m_upReverseTab->end());

    return iter->second;
}

void SymbolTable::addSymbol(wstring const & wstrName, Symbol const & sym)
{
    if (!m_upSymbolTab)
    {
        m_upSymbolTab  = make_unique<map <wstring, Symbol, std::less<>>>();
        m_upReverseTab = make_unique<map <Symbol, wstring>>();
    }
    (*m_upSymbolTab.get())[wstrName] = sym;
    (*m_upReverseTab.get())[sym] = wstrName;
}

void SymbolTable::Clear()
{
	m_upSymbolTab->clear();
	m_upReverseTab->clear();
}
