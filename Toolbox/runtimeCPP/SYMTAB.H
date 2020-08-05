//                                                                         
//  File:     SYMTAB.H                                                     
//                                                                         
// symtab.h - interface of symbol table 
//

#pragma once
          
#include <map>
#include "assert.h"
#include "script.h"

using std::map;

enum class tSTYPE   // Type of symbolic value 
{ 
    UnknownSTYPE,        // symbol is not defined  
    Function,            // name of a function     
    ULongConst,          // unsigned long integer  
    LongConst,           // long integer           
    FloatConst,          // float value            
    StringConst          // string value           
}; 

class Symbol
{
public:

    Symbol( );

    explicit Symbol( Script_Functor const * const pFunc ) : m_type( tSTYPE::Function    ), m_pFunc( pFunc   ), m_lValue( 0 ), m_ulValue( 0  ), m_wstrValue( L""  ), m_dValue( .0 ) { };
    explicit Symbol( long                   const l     ) : m_type( tSTYPE::LongConst   ), m_pFunc( nullptr ), m_lValue( l ), m_ulValue( 0  ), m_wstrValue( L""  ), m_dValue( .0 ) { };
    explicit Symbol( unsigned long          const ul    ) : m_type( tSTYPE::ULongConst  ), m_pFunc( nullptr ), m_lValue( 0 ), m_ulValue( ul ), m_wstrValue( L""  ), m_dValue( .0 ) { };
    explicit Symbol( wstring        const &       wstr  ) : m_type( tSTYPE::StringConst ), m_pFunc( nullptr ), m_lValue( 0 ), m_ulValue( 0  ), m_wstrValue( wstr ), m_dValue( .0 ) { };
    explicit Symbol( double                 const d     ) : m_type( tSTYPE::FloatConst  ), m_pFunc( nullptr ), m_lValue( 0 ), m_ulValue( 0  ), m_wstrValue( L""  ), m_dValue(  d ) { };

    bool operator< ( const Symbol & ) const;

    tSTYPE         const   GetSymbolType ( ) const { return m_type; };
    double         const   GetFloatConst ( ) const { assert( m_type == tSTYPE::FloatConst  ); return   m_dValue;    };
    Script_Functor const & GetFunction   ( ) const { assert( m_type == tSTYPE::Function    ); return * m_pFunc;     };
    wstring        const & GetStringConst( ) const { assert( m_type == tSTYPE::StringConst ); return   m_wstrValue; };
    long           const   GetLongConst  ( ) const { assert( m_type == tSTYPE::LongConst   ); return   m_lValue;    };
    unsigned long  const   GetUlongConst ( ) const { assert( m_type == tSTYPE::ULongConst  ); return   m_ulValue;   };

private:
    tSTYPE                 m_type;
    Script_Functor const * m_pFunc;     // type: Function    
    long                   m_lValue;    // type: LogConst    
    unsigned long          m_ulValue;   // type: ULongConst  
    wstring                m_wstrValue; // type: StringConst 
    double                 m_dValue;    // type: FloatConst  
};

class SymbolTable
{
public:

    static void ScrDefConst( wstring const &, Script_Functor const * const );
    static void ScrDefConst( wstring const &, wstring const &        );
    static void ScrDefConst( wstring const &, long const             );
    static void ScrDefConst( wstring const &, unsigned long const    );
    static void ScrDefConst( wstring const &, double const           );

    static Symbol  const & GetSymbolFromName( wstring const & );
    static wstring const & GetSymbolName    ( Symbol  const & );

	static void Clear( );

private:
    static void addSymbol( wstring const &, Symbol const & );

    inline static map < wstring const *, Symbol > m_SymbolTab  {};
    inline static map < Symbol, wstring const * > m_ReverseTab {};
};
