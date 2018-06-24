// win32_script.cpp
//

#include "stdafx.h"
#include "commdlg.h"
#include <string>
#include "SCRIPT.H"
#include "win32_script.h"

using namespace std;

wstring AskForScriptFileName( wstring wstrPath )
{
    OPENFILENAME ofn;                // common dialog box structure    
    wchar_t      lszFile[500];       // buffer for file name
    wstring      wstrRes;

    (void)wcscpy_s( lszFile, wstrPath.c_str( ) );
    (void)wcscpy_s( lszFile, L"\\*.in" );

    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize  = sizeof( OPENFILENAME );
    ofn.lpstrFile    = lszFile;
    ofn.nMaxFile     = sizeof( lszFile ) / sizeof( lszFile[0] );
    //lint -e840       Use of nul character in a string literal
    ofn.lpstrFilter  = L"Script files\0*.in\0";
    //lint +e840
    ofn.nFilterIndex = 1;

    if ( GetOpenFileName( &ofn ) )
        wstrRes = lszFile;

    return wstrRes;
}
