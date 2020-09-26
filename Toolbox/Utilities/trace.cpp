// trace.cpp
//
// Utilities

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include "trace.h"

wofstream OpenTraceFile( std::wstring const wszTraceFileName )
{
    wofstream traceStream;
    traceStream.open( wszTraceFileName, std::ios::out );
    if ( traceStream.bad() )
    {
        std::cout << "Could not open trace file " << std::endl;
    }

    return traceStream;
}
