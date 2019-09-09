// trace.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <string>
#include <fstream>
#include <iostream>
#include "trace.h"

std::wofstream OpenTraceFile( std::wstring const wszTraceFileName )
{
    std::wofstream traceStream;
    traceStream.open( wszTraceFileName, std::ios::out );
    if ( traceStream.bad() )
    {
        std::cout << "Could not open trace file " << std::endl;
        assert( false );
    }

//    traceStream << L"OpenTraceFile " << L" \"" << wszTraceFileName << L"\"" << endl;

    return traceStream;
}
