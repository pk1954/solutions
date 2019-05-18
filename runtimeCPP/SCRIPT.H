//                                                                 
//  File:     SCRIPT.H                                  
//                                   
//
// script.h - interface for runtime package of TESTTOOL
//

#pragma once

#include <string>
#include "scanner.h"

using std::wstring;

#define DEF_FUNC(name)        SymbolTable::ScrDefConst( L#name, new Wrap##name )
#define DEF_ULONG_CONST(name) SymbolTable::ScrDefConst( L#name, static_cast<unsigned long>(name) )

class Script;

class Script_Functor
{
public:
    virtual ~Script_Functor() {};
    virtual void operator() ( Script & script ) const = 0;
};

class Script
{
public:
// functions for reading from input script

    double         ScrReadFloat  ( void ); 
    long           ScrReadLong   ( void ); 
    unsigned long  ScrReadUlong  ( void ); 
    int            ScrReadInt    ( void ); 
    unsigned int   ScrReadUint   ( void ); 
    short          ScrReadShort  ( void ); 
    unsigned short ScrReadUshort ( void );
    wchar_t        ScrReadChar   ( void ); 
    unsigned char  ScrReadUchar  ( void ); 
    wstring const  ScrReadString ( void );
	void           ScrReadSpecial( wchar_t const ); 

// Helper routines for error handlers 
	wstring const Script::GetActPath         ( ) const { return m_pScanAct->GetActPath      ( ); }
	wstring const Script::GetActLine         ( ) const { return m_pScanAct->GetActLine      ( ); }
	int     const Script::GetActLineNr       ( ) const { return m_pScanAct->GetActLineNr    ( ); }
	int     const Script::GetActStartPos     ( ) const { return m_pScanAct->GetActStartPos  ( ); }
	int     const Script::GetActEndPos       ( ) const { return m_pScanAct->GetActEndPos    ( ); }
	wstring const Script::GetActExpectedToken( ) const { return m_pScanAct->GetExpectedToken( ); }
	bool    const Script::IsActive           ( ) const { return m_pScanAct != nullptr;           }

// utility functions

	static bool ProcessScript( wstring const & );
    static void ScrSetWrapHook( Script_Functor const * const );
	static void Clear( );

// script processing

    bool ScrProcess( wstring const & );

	static void StopProcessing( ) 
	{ 
		m_bStop = true; 
	}

private:

    bool          readSign( void );
    unsigned long numeric( wstring const &, unsigned long, bool * );

    Scanner * m_pScanAct;   // Pointer to active stack element
	
	static bool                   m_bStop;
	static Script_Functor const * m_pWrapHook;
};

class WrapInclude : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const;
};
