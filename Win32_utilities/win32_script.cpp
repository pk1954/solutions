// win32_script.cpp
//

#include "stdafx.h"
#include "commdlg.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <string>
#include "SCRIPT.H"
#include "win32_script.h"

using std::wstring;
using std::wostringstream;
using std::endl;

wstring AskForFileName
( 
	wstring       path, 
	wstring const filter, 
	wstring const description 
)
{
	static int const MAXPATH = 512;

	wstring wBufferPath   { path + L"\\" + filter };
	wstring wBufferFilter { description + wstring(L"\0", 1) + filter + wstring(L"\0\0", 2) };
	path.resize( MAXPATH );

    OPENFILENAME ofn;                  // common dialog box structure    
	ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize  = sizeof( OPENFILENAME );
    ofn.lpstrFile    = &path[0];
    ofn.nMaxFile     = MAXPATH;
    ofn.lpstrFilter  = wBufferFilter.c_str();
    ofn.nFilterIndex = 1;

    if ( ! GetOpenFileName( &ofn ) )
		path = L"";

	wstring wstrRes( path.c_str() );
	return wstrRes;
}
