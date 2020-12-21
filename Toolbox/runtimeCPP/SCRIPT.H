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
	void           ScrReadString ( wstring const );
	void           ScrReadSpecial( wchar_t const ); 

// Helper routines for error handlers 
	wstring const GetActPath         ( ) const { return m_pScanAct->GetActPath      ( ); }
	wstring const GetActLine         ( ) const { return m_pScanAct->GetActLine      ( ); }
	int     const GetActLineNr       ( ) const { return m_pScanAct->GetActLineNr    ( ); }
	int     const GetActStartPos     ( ) const { return m_pScanAct->GetActStartPos  ( ); }
	int     const GetActEndPos       ( ) const { return m_pScanAct->GetActEndPos    ( ); }
	wstring const GetActExpectedToken( ) const { return m_pScanAct->GetExpectedToken( ); }
	bool    const IsActive           ( ) const { return m_pScanAct != nullptr;           }

// utility functions

	Scanner & GetScanner( ) { return * m_pScanAct; }

	long long const GetFilePos    () const { return m_pScanAct->GetFilePos( ); };
	uintmax_t const GetFileSize   () const { return m_fileSize; };
	long      const GetPercentRead() const;

	static bool ProcessScript( wstring const & );
    static void ScrSetWrapHook( Script_Functor const * const );
	static void Clear( );

// script processing

    bool ScrProcess( wstring const & );

	static void StopProcessing( ) { m_bStop = true; }

private:

    bool          readSign( void );
    unsigned long numeric( wstring const &, unsigned long, bool * );

	Scanner * m_pScanAct { nullptr };   // Pointer to active stack element
	
	inline static bool                   m_bStop     { false };
	inline static Script_Functor const * m_pWrapHook { nullptr };
	
	uintmax_t m_fileSize { 0 };
};

class WrapInclude : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const;
};
