// EvolutionCoreTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include "SCRIPT.H"
#include "wrappers.h"

//lint -esym( 715, argc )    symbol not referenced
//lint -esym( 715, argv )    symbol not referenced

int _tmain( int const argc, _TCHAR const * const argv[] )
{
    //lint -e826    Suspicious pointer-to-pointer conversion (area too small)
    wstring const inputFile = (argc > 1) ? static_cast<wchar_t const *>(argv[1]) : L"test.in";

    DefineWrapperFunctions( );

    wofstream traceStream;  
    traceStream.open( L"main_trace.out", ios::out );
    assert( traceStream.good() );
    ScrSetOutputStream( &traceStream ); 

    cout << "Processing input file ";
    wcout << inputFile << endl;

    int const iRetCode = ScrProcess( inputFile ) ? 0 : 2;

    cout << "return code: " << iRetCode << endl;
}
