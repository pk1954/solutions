// Script.h                                  
//
// Toolbox\runtimeCPP

#pragma once

#include <string>
#include <stdint.h>
#include "Scanner.h"

using std::wstring;

class Script;

class ScriptFunctor
{
public:
	virtual ~ScriptFunctor() = default;
	virtual void operator() (Script & script) const = 0;
};

class Script
{
public:
	
	// functions for reading from input script

    double         ScrReadFloat  (void); 
    long           ScrReadLong   (void); 
    unsigned long  ScrReadUlong  (void); 
    int            ScrReadInt    (void); 
    unsigned int   ScrReadUint   (void); 
    short          ScrReadShort  (void); 
    unsigned short ScrReadUshort (void);
    wchar_t        ScrReadChar   (void); 
    unsigned char  ScrReadUchar  (void); 
	wstring        ScrReadString (void);
	void           ScrReadString (wstring const &);
	void           ScrReadSpecial(wchar_t const); 

// Helper routines for error handlers 

	void SetExpectedToken(wstring const & wstrExp) { m_scanner.SetExpectedToken(wstrExp); }

	wstring GetActPath         () const { return m_scanner.GetActPath      (); }
	wstring GetActLine         () const { return m_scanner.GetActLine      (); }
	int     GetActLineNr       () const { return m_scanner.GetActLineNr    (); }
	int     GetActStartPos     () const { return m_scanner.GetActStartPos  (); }
	int     GetActEndPos       () const { return m_scanner.GetActEndPos    (); }
	wstring GetActExpectedToken() const { return m_scanner.GetExpectedToken(); }
	bool    IsActive           () const { return m_scanner.IsActive        (); }

// utility functions

	Scanner & GetScanner() { return m_scanner; }

	long      GetPercentRead();
	long long GetFilePos    ()       { return m_scanner.GetFilePos(); };
	uintmax_t GetFileSize   () const { return m_fileSize; };

	static void ScrSetWrapHook(ScriptFunctor const * const);
	static void Clear();

	void ScrSetNewLineHook(ScriptFunctor const * const);

// script processing

	bool ScrOpen(wstring const &);
	bool ReadNextToken();
	bool ProcessToken();
	bool ScrClose();
	bool ScrProcess(wstring const &);

	void SetEcho(bool const bMode) { m_bEchoScript = bMode; }

	static void StopProcessing() { m_bStop = true; }

private:

    bool          readSign(void);
    unsigned long numeric(wstring const &, unsigned long, bool *);

	Scanner   m_scanner; 
	tTOKEN    m_token       { tTOKEN::End };
	bool      m_bEchoScript { true };
	uintmax_t m_fileSize    { 0 };

	inline static bool                  m_bStop        { false };
	inline static ScriptFunctor const * m_pWrapHook    { nullptr };
	inline static ScriptFunctor const * m_pNewLineHook { nullptr };
};

class WrapInclude : public ScriptFunctor
{
public:
    void operator() (Script & script) const final;
};
