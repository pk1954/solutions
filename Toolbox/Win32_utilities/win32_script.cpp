// win32_script.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "commdlg.h"
#include <assert.h> 
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

IFileDialog * ScriptFile::m_pFileDlgOpen { nullptr };
IFileDialog * ScriptFile::m_pFileDlgSave { nullptr };

wstring const ScriptFile::GetPathOfExecutable( )
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

ScriptFile::ScriptFile( )
{
    HRESULT hr = CoInitializeEx( NULL, COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE );
    assert( SUCCEEDED(hr) );

    hr = CoCreateInstance 
    (
        CLSID_FileOpenDialog, 
        NULL, CLSCTX_ALL, 
        IID_IFileOpenDialog, 
        reinterpret_cast<void**>( & m_pFileDlgOpen )
    );
    assert( SUCCEEDED(hr) );

    hr = CoCreateInstance
    (
        CLSID_FileSaveDialog, 
        NULL, CLSCTX_ALL, 
        IID_IFileSaveDialog, 
        reinterpret_cast<void**>( & m_pFileDlgSave )
    );
    assert( SUCCEEDED(hr) );

    hr = m_pFileDlgOpen->SetTitle( L"Open file" );
    assert( SUCCEEDED(hr) );
    hr = m_pFileDlgSave->SetTitle( L"Save file" );
    assert( SUCCEEDED(hr) );
}

ScriptFile::~ScriptFile( )
{
    m_pFileDlgOpen->Release();
    m_pFileDlgSave->Release();
    CoUninitialize();
}

wstring ScriptFile::getResult( IFileDialog * const pFileDlg )
{
    HRESULT      hr;
    wstring      wstrRes { };
    IShellItem * pItem   { nullptr };
    hr = pFileDlg->GetResult( & pItem );                        
    if ( SUCCEEDED(hr) )
    {
        PWSTR pszPath { nullptr };
        hr = pItem->GetDisplayName( SIGDN_FILESYSPATH, & pszPath ); 
        if ( SUCCEEDED(hr) )
        {
            wstrRes = pszPath;
            CoTaskMemFree( pszPath );
        }
        pItem->Release();
    }
    return wstrRes;
}

wstring const ScriptFile::AskForFileName
( 
	wstring   const extension, 
	wstring   const description,
	tFileMode const mode
)
{
    wstring           filter   { L"*." + extension };
    COMDLG_FILTERSPEC rgSpec   { description.c_str(), filter.c_str() };
    IFileDialog     * pFileDlg { (mode == tFileMode::read) ? m_pFileDlgOpen : m_pFileDlgSave };
    wstring           wstrRes  { };
    HRESULT           hr;
    hr = m_pFileDlgSave->SetDefaultExtension( extension.c_str() ); assert( SUCCEEDED(hr) );
    hr = pFileDlg->SetFileTypes( 1, & rgSpec );                    assert( SUCCEEDED(hr) );
    hr = pFileDlg->Show( NULL ); 
    if ( SUCCEEDED(hr) )
        wstrRes =  getResult( pFileDlg );
    return wstrRes;
}
