// Preferences.cpp
//
// NNetWindows

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "SCRIPT.H"
#include "ERRHNDL.H"
#include "Resource.h"
#include "AutoOpen.h"
#include "SoundInterface.h"
#include "NNetModelImporter.h"
#include "NNetModelReaderInterface.h"
#include "NNetParameters.h"
#include "win32_MainWindow.h"
#include "win32_descriptionWindow.h"
#include "win32_importTermination.h"
#include "win32_NNetAppMenu.h"
#include "win32_script.h"
#include "Preferences.h"

using std::wofstream;
using std::wstring;
using std::string;
using std::wcout;
using std::filesystem::exists;
using std::filesystem::path;

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

static wstring m_wstrPreferencesFile;

class WrapShowArrows: public WrapBase
{
public:
    WrapShowArrows(MainWindow & mainWin)
      : WrapBase(L"ShowArrows"),
        m_mainWin(mainWin)
    {}

    void operator() (Script & script) const final
    {
        m_mainWin.ShowArrows(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream & out) const final
    {
        out << (m_mainWin.ArrowsVisible() ? PREF_ON : PREF_OFF);
    }

private:
    MainWindow & m_mainWin;
};

class WrapShowSensorPoints: public WrapBase
{
public:
    WrapShowSensorPoints(MainWindow & mainWin)
      : WrapBase(L"ShowSensorPoints"),
        m_mainWin(mainWin)
    {}

    void operator() (Script & script) const final
    {
        m_mainWin.ShowSensorPoints(static_cast<bool>(script.ScrReadUint()));
    }

    void Write(wostream & out) const final
    {
        out << (m_mainWin.SensorsPointsVisible() ? PREF_ON : PREF_OFF);
    }

private:
    MainWindow & m_mainWin;
};

class WrapSetAutoOpen: public WrapBase
{
public:
    WrapSetAutoOpen()
        : WrapBase(L"SetAutoOpen")
    {}

    void operator() (Script & script) const final
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if (bMode)
            AutoOpen::On();
        else
            AutoOpen::Off();
    }

    void Write(wostream & out) const final
    {
        out << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetSound: public WrapBase
{
public:
    explicit WrapSetSound(Sound & sound)
      : WrapBase(L"SetSound"),
        m_sound(sound)
    {}

    void operator() (Script & script) const final
    {
        bool bMode { static_cast<bool>(script.ScrReadUint()) };
        if (bMode)
            m_sound.On();
        else
            m_sound.Off();
    }

    void Write(wostream & out) const final
    {
        out << (m_sound.IsOn() ? PREF_ON : PREF_OFF);
    }

private:
    Sound & m_sound;
};

class WrapDescWinFontSize: public WrapBase
{
public:
    explicit WrapDescWinFontSize(DescriptionWindow & descWin)
      : WrapBase(L"DescWinFontSize"),
        m_descWin(descWin)
    {}

    void operator() (Script & script) const final
    {
        m_descWin.SetFontSize(script.ScrReadInt());
    }

    void Write(wostream & out) const final
    {
        out << m_descWin.GetFontSize();
    }

private:
    DescriptionWindow & m_descWin;
};

class WrapReadModel: public WrapBase
{
public:
    WrapReadModel
    (
        NNetModelReaderInterface & nmri,
        NNetModelImporter        & modelImporter, 
        HWND               const   hwndApp
    )
    : WrapBase(L"ReadModel"),
      m_nmri(nmri),
      m_modelImporter(modelImporter),
      m_hwndApp(hwndApp)
    {}

    void operator() (Script & script) const final
    {
        wstring wstrFile    { script.ScrReadString() };
        auto    termination { make_unique<NNetImportTermination>(m_hwndApp, IDX_REPLACE_MODEL) };
        if (! m_modelImporter.Import(wstrFile, move(termination)))
        {
            SendMessage(m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0);
        }
     }

    void Write(wostream & out) const final
    {
        out <<  L"\""  << m_nmri.GetModelFilePath() << L"\"";
    }

private:
    NNetModelReaderInterface & m_nmri;
    NNetModelImporter        & m_modelImporter;
    HWND                       m_hwndApp;
};

void Preferences::Initialize
(
    NNetModelReaderInterface & nmri,
    DescriptionWindow        & descWin,
    MainWindow               & mainWin,
    Sound                    & sound, 
    NNetModelImporter        & modelImporter,
    HWND                       hwndApp
)
{
    wchar_t szBuffer[MAX_PATH];
    DWORD const dwRes = GetCurrentDirectory(MAX_PATH, szBuffer);
    assert(dwRes > 0);

    static wstring const PREFERENCES_FILE_NAME { L"NNetSimu_UserPreferences.txt" };

    m_wstrPreferencesFile = szBuffer;
    m_wstrPreferencesFile += L"\\" + PREFERENCES_FILE_NAME;
    
    m_prefVector.push_back(make_unique<WrapShowArrows>(mainWin));
    m_prefVector.push_back(make_unique<WrapShowSensorPoints>(mainWin));
    m_prefVector.push_back(make_unique<WrapDescWinFontSize>(descWin));
    m_prefVector.push_back(make_unique<WrapSetAutoOpen>());
    m_prefVector.push_back(make_unique<WrapSetSound>(sound));
    m_prefVector.push_back(make_unique<WrapReadModel>(nmri, modelImporter, hwndApp));

    SymbolTable::ScrDefConst(PREF_OFF, 0L);
    SymbolTable::ScrDefConst(PREF_ON,  1L);
}

bool Preferences::ReadPreferences() const
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

bool Preferences::WritePreferences() const
{
    wofstream prefFile(m_wstrPreferencesFile);
    for (auto const & it : m_prefVector)
        it->Write2(prefFile);
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
