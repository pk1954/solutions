// EvolutionCoreTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "version.h"
#include <iostream>
#include "config.h"
#include "trace.h"
#include "win32_hiResTimer.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridNeighbor.h"

class gridNeighborTest : public GridPointNeighbor_Functor
{
public:
    gridNeighborTest( GridPoint const & gp ) 
	: GridPointNeighbor_Functor( gp )
    { }

    virtual ~gridNeighborTest() { };
 
    virtual bool operator() ( GridPoint const & gpNeighbor ) const 
    {
        return false;
    }
};

class visitNeighbours : public GridPoint_Functor
{
public:
    visitNeighbours( ) 
    : GridPoint_Functor( )
    { };

	virtual ~visitNeighbours() { };

    virtual void operator() ( GridPoint const & gp )
    {
		Apply2AllNeighbors( gridNeighborTest( gp ) );
    }
};

class gpFunctorEmpty : public GridPoint_Functor
{
public:
    gpFunctorEmpty( ) 
    : GridPoint_Functor( )
    { };

	virtual ~gpFunctorEmpty() { };

    virtual void operator() ( GridPoint const & gp )
    { }
};

int const NRUNS = 5000;

void testee()
{
	for ( int i = 0; i <= NRUNS; ++i )
		Apply2Grid( & visitNeighbours( ) );
}

void tara()
{
	for ( int i = 0; i <= NRUNS; ++i )
		Apply2Grid( & gpFunctorEmpty( ) );
}

void DoTest( )
{
	HiResTimer m_hrtimer;
	wofstream  m_traceStream = OpenTraceFile( L"main_trace.out" );

	m_hrtimer.Start( );
	tara( );
	m_hrtimer.Stop( );

	DWORD dwMicroSecsTara = m_hrtimer.Get( );
	DWORD dwMilliSecsTara = dwMicroSecsTara / 1000;

	m_hrtimer.Start( );
	testee( );
	m_hrtimer.Stop( );

	DWORD dwMicroSecsBrutto = m_hrtimer.Get( );
	DWORD dwMilliSecsBrutto = dwMicroSecsBrutto / 1000;

	DWORD dwMicroSecsNetto = dwMicroSecsBrutto - dwMicroSecsTara;
	DWORD dwMilliSecsNetto = dwMicroSecsNetto / 1000;

	wcout << L"Brutto " << dwMilliSecsBrutto << L" msecs" << endl;
	wcout << L"Tara   " << dwMilliSecsTara   << L" msecs" << endl;
	wcout << L"Netto  " << dwMilliSecsNetto  << L" msecs" << endl;
	wcout << dwMicroSecsNetto / NRUNS << L" usecs/run" << endl;

	wcout << endl << L"*** EvolutionCoreTest finished" << endl;
}

int _tmain( int const argc, _TCHAR const * const argv[] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

    wstring const inputFile = (argc > 1) ? static_cast<wchar_t const *>(argv[1]) : L"test.in";
	
	try
	{
		DoTest( );
	}
	catch (...)
	{
		wcout << endl << L"+++ Error in EvolutionCoreTest" << endl;
		return 1;
	}

	return 0;

}
