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
#include "Resource.h"
#include "AutoOpen.h"
#include "SoundInterface.h"
#include "NNetModelImporter.h"
#include "NNetParameters.h"
#include "win32_importTermination.h"
#include "win32_NNetAppMenu.h"
#include "Preferences.h"

using std::wofstream;
using std::wstring;
using std::string;
using std::wcout;
using std::endl;
using std::filesystem::exists;
using std::filesystem::path;

static wstring m_wstrPreferencesFile;

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
    WrapSetSound( Sound & sound )
        : m_sound( sound )
    {}

    virtual void operator() ( Script & script ) const
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if ( bMode )
            m_sound.On();
        else
            m_sound.Off();
    }

private:
    Sound & m_sound;
};

class WrapReadModel: public Script_Functor
{
public:
    WrapReadModel( NNetModelImporter & modelImporter, HWND const hwndApp )
        : m_modelImporter( modelImporter )
    {
        m_hwndApp = hwndApp;
    }

    virtual void operator() ( Script & script ) const
    {
        wstring wstrFile    { script.ScrReadString() };
        auto    termination { make_unique<NNetImportTermination>( m_hwndApp, IDX_REPLACE_MODEL ) };
        if ( ! m_modelImporter.Import( wstrFile, move(termination) ) )
        {
            SendMessage( m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0 );
        }
     }

private:
    NNetModelImporter & m_modelImporter;
    HWND                m_hwndApp;
};

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

wstring const PREFERENCES_FILE_NAME { L"NNetSimu_UserPreferences.txt" };

void Preferences::Initialize
( 
    Sound             & sound, 
    NNetModelImporter & modelImporter,
    HWND                hwndApp
)
{
    m_pSound  = & sound;
    m_hwndApp = hwndApp;

    wchar_t szBuffer[MAX_PATH];
    DWORD const dwRes = GetCurrentDirectory( MAX_PATH, szBuffer );
    assert( dwRes > 0 );

    m_wstrPreferencesFile = szBuffer;
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    SymbolTable::ScrDefConst( L"SetAutoOpen", new WrapSetAutoOpen( ) );
    SymbolTable::ScrDefConst( L"SetSound",    new WrapSetSound ( sound ) );
    SymbolTable::ScrDefConst( L"ReadModel",   new WrapReadModel( modelImporter, m_hwndApp ) );

    SymbolTable::ScrDefConst( PREF_OFF, 0L );
    SymbolTable::ScrDefConst( PREF_ON,  1L );
}

bool Preferences::ReadPreferences( )
{
    if ( exists( m_wstrPreferencesFile ) )
    {
        wcout << L"*** read preferences file " << m_wstrPreferencesFile << endl;
        return Script::ProcessScript( m_wstrPreferencesFile );
    }
    else 
    {
        wcout << L"+++ preferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << L"+++ using defaults" << endl;
        return false;
    }
}

bool Preferences::WritePreferences( wstring const wstrModelPath )
{
    wofstream prefFile( m_wstrPreferencesFile );
    prefFile << L"SetSound "    << (m_pSound->IsOn() ? PREF_ON : PREF_OFF) << endl;
	prefFile << L"SetAutoOpen " << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF) << endl;
    prefFile << L"ReadModel \"" << wstrModelPath << L"\"" << endl;
    prefFile.close( );
    wcout << L"*** preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
