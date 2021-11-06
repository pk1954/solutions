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
#include "win32_descriptionWindow.h"
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

class WrapSetAutoOpen;
class WrapSetSound;
class WrapDescWinFontSize;
class WrapReadModel;

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

static wstring m_wstrPreferencesFile;

static unique_ptr<WrapSetAutoOpen    > m_upWrapSetAutoOpen     {};
static unique_ptr<WrapSetSound       > m_upWrapSetSound        {};
static unique_ptr<WrapDescWinFontSize> m_upWrapDescWinFontSize {};
static unique_ptr<WrapReadModel      > m_upWrapReadModel       {};

class WrapBase : public ScriptFunctor
{
public:
    void SetName(wstring const & wstrName)
    {
        m_wstrName = wstrName;
        SymbolTable::ScrDefConst(wstrName, this);
    }

    void Write(wostream & out)
    {
        out << m_wstrName << L" ";
    }

private:
    wstring m_wstrName;
};


class WrapSetAutoOpen: public WrapBase
{
public:
    virtual void operator() (Script & script) const
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if (bMode)
            AutoOpen::On();
        else
            AutoOpen::Off();
    }

    void Write(wostream& out)
    {
        WrapBase::Write(out);
        out << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF) << endl;
    }
};

class WrapSetSound: public WrapBase
{
public:
    WrapSetSound(Sound & sound)
        : m_sound(sound)
    {}

    virtual void operator() (Script & script) const
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if (bMode)
            m_sound.On();
        else
            m_sound.Off();
    }

    void Write(wostream& out)
    {
        WrapBase::Write(out);
        out << (m_sound.IsOn() ? PREF_ON : PREF_OFF) << endl;
    }

private:
    Sound & m_sound;
};

class WrapDescWinFontSize: public WrapBase
{
public:
    WrapDescWinFontSize(DescriptionWindow & descWin)
      : m_descWin(descWin)
    {}

    virtual void operator() (Script & script) const
    {
        m_descWin.SetFontSize(script.ScrReadInt());
    }

    void Write(wostream& out)
    {
        WrapBase::Write(out);
        out << m_descWin.GetFontSize() << endl;
    }

private:
    DescriptionWindow & m_descWin;
};

class WrapReadModel: public WrapBase
{
public:
    WrapReadModel
    (
        NNetModelImporter & modelImporter, 
        HWND        const   hwndApp
    )
    : m_modelImporter(modelImporter),
      m_hwndApp(hwndApp)
    {}

    virtual void operator() (Script & script) const
    {
        wstring wstrFile    { script.ScrReadString() };
        auto    termination { make_unique<NNetImportTermination>(m_hwndApp, IDX_REPLACE_MODEL) };
        if (! m_modelImporter.Import(wstrFile, move(termination)))
        {
            SendMessage(m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0);
        }
     }

    void Write(wostream& out, wstring const wstrModelPath)
    {
        WrapBase::Write(out);
        out <<  L"\""  << wstrModelPath << L"\"" << endl;
    }

private:
    NNetModelImporter & m_modelImporter;
    HWND                m_hwndApp;
};

void Preferences::Initialize
(
    DescriptionWindow & descWin,
    Sound             & sound, 
    NNetModelImporter & modelImporter,
    HWND                hwndApp
)
{
    m_pDescWin = & descWin;
    m_pSound   = & sound;
    m_hwndApp  = hwndApp;

    wchar_t szBuffer[MAX_PATH];
    DWORD const dwRes = GetCurrentDirectory(MAX_PATH, szBuffer);
    assert(dwRes > 0);

    static wstring const PREFERENCES_FILE_NAME { L"NNetSimu_UserPreferences.txt" };

    m_wstrPreferencesFile = szBuffer;
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    m_upWrapDescWinFontSize = make_unique<WrapDescWinFontSize>(descWin);
    m_upWrapSetAutoOpen     = make_unique<WrapSetAutoOpen    >();
    m_upWrapSetSound        = make_unique<WrapSetSound       >(sound);
    m_upWrapReadModel       = make_unique<WrapReadModel      >(modelImporter, m_hwndApp);

    m_upWrapDescWinFontSize->SetName(L"DescWinFontSize");
    m_upWrapSetAutoOpen    ->SetName(L"SetAutoOpen");
    m_upWrapSetSound       ->SetName(L"SetSound");
    m_upWrapReadModel      ->SetName(L"ReadModel");

    SymbolTable::ScrDefConst(PREF_OFF, 0L);
    SymbolTable::ScrDefConst(PREF_ON,  1L);
}

bool Preferences::ReadPreferences()
{
    if (exists(m_wstrPreferencesFile))
    {
        wcout << Scanner::COMMENT_START << L"Read preferences file " << m_wstrPreferencesFile << endl;
        Script script;
        script.SetEcho(false);
        return script.ScrProcess(m_wstrPreferencesFile);
    }
    else 
    {
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Preferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Using defaults" << endl;
        return false;
    }
}

bool Preferences::WritePreferences(wstring const wstrModelPath)
{
    wofstream prefFile(m_wstrPreferencesFile);
    m_upWrapSetAutoOpen    ->Write(prefFile);
    m_upWrapSetSound       ->Write(prefFile);
    m_upWrapDescWinFontSize->Write(prefFile);
    m_upWrapReadModel      ->Write(prefFile, wstrModelPath);
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
