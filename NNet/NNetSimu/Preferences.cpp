// Preferences.cpp
//
// NNetWindows

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Script.h"
#include "errhndl.h"
#include "symtab.h"
#include "AutoOpen.h"
#include "NNetModelStorage.h"
#include "NNetParameters.h"
#include "win32_NNetAppMenu.h"
#include "Win32_sound.h"
#include "Win32_NNetWindow.h"
#include "Preferences.h"

using std::wofstream;
using std::wstring;
using std::string;
using std::wcout;
using std::endl;
using std::filesystem::exists;

static NNetModelStorage * m_pNNetModelStorage;
static wstring            m_wstrPreferencesFile;

class WrapSetAutoOpen: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if ( bMode )
            AutoOpen::On();
        else
            AutoOpen::Off();
   }
};

class WrapSetSound: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if ( bMode )
            Sound::On();
        else
            Sound::Off();
    }
};

class WrapReadModel: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        if ( ! m_pNNetModelStorage->Read( script.ScrReadString() ) )
            ScriptErrorHandler::semanticError( L"Error in model file." );
    }
};

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

wstring const PREFERENCES_FILE_NAME { L"NNetSimu_UserPreferences.txt" };

void Preferences::Initialize( )
{
    wchar_t szBuffer[MAX_PATH];
    DWORD const dwRes = GetCurrentDirectory( MAX_PATH, szBuffer );
    assert( dwRes > 0 );

    m_wstrPreferencesFile = szBuffer;
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    DEF_FUNC( SetAutoOpen );
    DEF_FUNC( SetSound );
    DEF_FUNC( ReadModel );

    SymbolTable::ScrDefConst( PREF_OFF, 0L );
    SymbolTable::ScrDefConst( PREF_ON,  1L );
}

bool Preferences::ReadPreferences( NNetModelStorage * pStorage )
{
    m_pNNetModelStorage = pStorage;
    if ( exists( m_wstrPreferencesFile ) )
    {
        wcout << L"*** preferences file " << m_wstrPreferencesFile;
        wcout << L" opened" << endl;
        bool bRes { Script::ProcessScript( m_wstrPreferencesFile ) };
        if ( bRes )
            wcout << L"*** processed successfully " << endl;
        return bRes;
    }
    else 
    {
        wcout << L"+++ preferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << L"+++ using defaults" << endl;
        return false;
    }
}

bool Preferences::WritePreferences( wstring const wstrModelPath, NNetWindow const * const pNNetWindow )
{
    wofstream prefFile( m_wstrPreferencesFile );
    prefFile << L"SetSound "       << (Sound   ::IsOn() ? PREF_ON : PREF_OFF) << endl;
	prefFile << L"SetAutoOpen "    << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF) << endl;
    prefFile << L"ReadModel \""    << wstrModelPath << L"\"" << endl;
    prefFile << L"SetPixelOffset " << pNNetWindow->GetPixelOffset() << endl;
    prefFile << L"SetPixelSize "   << pNNetWindow->GetPixelSize() << endl;
    prefFile.close( );
    wcout << L"*** preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
