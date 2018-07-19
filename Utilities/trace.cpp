// trace.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <string>
#include <fstream>
#include <iostream>
#include "trace.h"

using namespace std;

wofstream OpenTraceFile( wstring const wszTraceFileName )
{
    wofstream traceStream;
    traceStream.open( wszTraceFileName, ios::out );
    if ( traceStream.bad() )
    {
        cout << "Could not open trace file " << endl;
        assert( false );
    }

//    traceStream << L"OpenTraceFile " << L" \"" << wszTraceFileName << L"\"" << endl;

    return traceStream;
}
