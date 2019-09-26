// EvolutionCoreTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "version.h"
#include <chrono>
#include <iostream>
#include "config.h"
#include "trace.h"
#include "script.h"
#include "win32_hiResTimer.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "GridDimensions.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"

using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::wofstream;
using std::wstring;
using std::wcout;
using std::endl;

int const NRUNS = 2000;

EvolutionCore * pCore;

void testee()
{
	for ( int i = 0; i <= NRUNS; ++i )
	{
		pCore->Compute( );
	}
}

void tara()
{
	for ( int i = 0; i <= NRUNS; ++i )
	{ }
}

void DoTest( )
{
	HiResTimer m_hrtimer;
	wofstream  m_traceStream = OpenTraceFile( L"main_trace.out" );

	Config::SetDefaultConfiguration( );
	Config::DefineConfigWrapperFunctions( );
	Script::ProcessScript( L"std_configuration.in" );
	GridDimensions::DefineGridSize
	( 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridWidth ) }, 
		GRID_COORD{ Config::GetConfigValueShort( Config::tId::gridHeight ) }, 
		Config::GetConfigValue( Config::tId::nrOfNeighbors ) 
	);
	EvolutionCore::InitClass( GridDimensions::GetNrOfNeigbors(), nullptr, nullptr );

	pCore = EvolutionCore::CreateCore( );
	DefineCoreWrapperFunctions( pCore );

	Script::ProcessScript( L"std_script.in" );

	m_hrtimer.Start( );
	tara( );
	m_hrtimer.Stop( );

	microseconds usTara = m_hrtimer.GetAndReset( );
	milliseconds msTara = duration_cast<milliseconds>(usTara);

	m_hrtimer.Start( );
	testee( );
	m_hrtimer.Stop( );

	wcout << L"Gen " << pCore->GetEvoGenerationNr().GetValue() << L" " << pCore->GetNrOfLivingIndividuals( ) << L" individuals alive" << endl;

	microseconds usBrutto = m_hrtimer.GetAndReset( );
	milliseconds msBrutto = duration_cast<milliseconds>(usBrutto);

	microseconds usNetto = usBrutto - usTara;
	milliseconds msNetto = duration_cast<milliseconds>(usNetto);

	wcout << L"Brutto " << msBrutto.count() << L" msecs" << endl;
	wcout << L"Tara   " << msTara.count()   << L" msecs" << endl;
	wcout << L"Netto  " << msNetto.count()  << L" msecs" << endl;
	wcout << usNetto.count() / NRUNS << L" usecs/run" << endl;

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
