// win32_script.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "commdlg.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <vector>
#include "pathcch.h"
#include "SCRIPT.H"
#include "win32_stopwatch.h"
#include "win32_script.h"

using std::vector;
using std::wstring;
using std::wostringstream;
using std::endl;

wstring GetPathOfExecutable( )
{
	int iBufSize { 256 };
	vector<wchar_t> buffer;

	do
	{
		iBufSize *= 2;
		buffer.resize( iBufSize );
	} while ( GetModuleFileName( nullptr, buffer.data(), iBufSize ) == iBufSize );

	return wstring( buffer.data() );
}

wstring AskForFileName // TODO: cleanup
( 
	wstring         pathSelected, // input: path to start 
	wstring   const filter, 
	wstring   const description,
	tFileMode const mode
)
{
	static int const MAXPATH = 512;

	wstring wBufferPath   { pathSelected };
	wstring wBufferFilter { description + wstring(L"\0", 1) + filter + wstring(L"\0\0", 2) };
	PCWSTR  strExtension  { nullptr };

	pathSelected = L"\0";
	pathSelected.resize( MAXPATH );
	wBufferPath += L'\0';

	HRESULT res = PathCchFindExtension( wBufferPath.c_str(), wBufferPath.size(), & strExtension );
	++strExtension;  // skip '.' befor extension
	PathCchRemoveFileSpec( &wBufferPath[0], MAXPATH );

    OPENFILENAME ofn;                  // common dialog box structure    
	ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize     = sizeof( OPENFILENAME );
	ofn.lpstrFile       = &pathSelected[0];
	ofn.lpstrDefExt     = strExtension;
	ofn.lpstrInitialDir = wBufferPath.c_str();
    ofn.nMaxFile        = MAXPATH;
    ofn.lpstrFilter     = wBufferFilter.c_str();
    ofn.nFilterIndex    = 1;

	bool bRes = (mode == tFileMode::read)
		        ? GetOpenFileName( &ofn )
		        : GetSaveFileName( &ofn );

	if ( bRes == false )
		pathSelected = L"";

	wstring wstrRes( pathSelected.c_str() );
	return wstrRes;
}

void ScriptDialog( )
{
	wchar_t szBuffer[MAX_PATH];
	DWORD const dwRes = GetCurrentDirectory( MAX_PATH, szBuffer);
	assert( dwRes > 0 );
	wstring const wstrPath( szBuffer );
	wstring wstrFile = AskForFileName( wstrPath, L"*.in", L"Script files", tFileMode::read );
	if ( ! wstrFile.empty( ) )
	{
		Stopwatch stopwatch;
		stopwatch.Start();
		std::wcout << L"Processing script file " << wstrFile << L"...";
		Script::ProcessScript( wstrFile );
		stopwatch.Stop( L"" );
	}
}
