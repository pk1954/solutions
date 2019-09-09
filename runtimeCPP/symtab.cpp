//
//  File:     symtab.cpp                                                    *
//                                                                          *
 
#include "stdafx.h"
#include <map>
#include <assert.h>
#include "errhndl.h"
#include "symtab.h"

std::map < wstring, Symbol  > SymbolTable::m_SymbolTab;
std::map < Symbol,  wstring > SymbolTable::m_ReverseTab;

Symbol::Symbol( ) :
    m_type(tSTYPE::UnknownSTYPE  ),
    m_pFunc( nullptr ),
    m_lValue( 0L ),    
    m_ulValue( 0L ),   
    m_wstrValue( L"" ),
    m_dValue( 0.0f )
{ };

bool Symbol::operator< ( const Symbol & rhs ) const
{
    if ( m_type < rhs.m_type ) 
        return true;
    
    switch ( m_type )
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
           assert( false );
    }
    return false;
}

// GetSymbolFromName: Find symbol in symbol table with error handling

Symbol const & SymbolTable::GetSymbolFromName( wstring const & wstrKey )
{
    auto const iter = m_SymbolTab.find( wstrKey );

    if ( iter == m_SymbolTab.end() )
        ScriptErrorHandler::symbolError( wstrKey );

    return iter->second;
}

wstring const & SymbolTable::GetSymbolName( Symbol const & symbol )
{
    auto const iter = m_ReverseTab.find( symbol );

    assert( iter != m_ReverseTab.end( ) );

    return iter->second;
}

void SymbolTable::addSymbol( wstring const & wstrName, Symbol const & sym )
{
 //   auto const iter = m_SymbolTab.find( wstrName );

	//assert( iter == m_SymbolTab.end() );  // duplicate definition of symbolic constant
 //                                       
    m_SymbolTab[ wstrName ] = sym;
    m_ReverseTab[ sym ] = wstrName;
}

void SymbolTable::ScrDefConst( wstring const & wstrName, Script_Functor const * const pFunc )
{
    addSymbol( wstrName, Symbol( pFunc ) );
}

void SymbolTable::ScrDefConst( wstring const & wstrName, long const lValue )
{
    addSymbol( wstrName, Symbol( lValue ) );
}

void SymbolTable::ScrDefConst( wstring const & wstrName, unsigned long const ulValue )
{
    addSymbol( wstrName, Symbol( ulValue ) );
}

void SymbolTable::ScrDefConst( wstring const & wstrName, double const dValue )
{
    addSymbol( wstrName, Symbol( dValue ) );
}

void SymbolTable::ScrDefConst( wstring const & wstrName, wstring const & wstrValue )
{
    addSymbol( wstrName, Symbol( wstrValue ) );
}

void SymbolTable::Clear()
{
	m_SymbolTab.clear();
	m_ReverseTab.clear();
}
