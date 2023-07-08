// Preferences.cpp
//
// Toolbox\runtimeCPP

module;

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

module Preferences;

import Symtab;
import Scanner;
import SoundInterface;
import AutoOpen;

using std::wofstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;
using std::make_unique;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::current_path;

static wstring const PREF_ON  { L"ON"  };
static wstring const PREF_OFF { L"OFF" };

class WrapPrefBase : public WrapBase
{
public:
    WrapPrefBase
    (
        wstring const& wstrName,
        Preferences& pref
    )
      : WrapBase(wstrName),
        m_pref(pref)
    {}

protected:
    Preferences& m_pref;
};

class WrapSetColorMenu : public WrapPrefBase
{
public:
    using WrapPrefBase::WrapPrefBase;

    void operator() (Script& script) const final
    {
        m_pref.SetColorMenu(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ColorMenuVisible() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetSound : public WrapPrefBase
{
public:
    using WrapPrefBase::WrapPrefBase;

    void operator() (Script& script) const final
    {
        bool bMode { script.ScrReadBool() };
        if (bMode)
            m_pref.GetSound().On();
        else
            m_pref.GetSound().Off();
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.GetSound().IsOn() ? PREF_ON : PREF_OFF);
    }
};

class WrapSetAutoOpen : public WrapPrefBase
{
public:
    using WrapPrefBase::WrapPrefBase;

    void operator() (Script& script) const final
    {
        bool bMode { script.ScrReadBool() };
        if (bMode)
            AutoOpen::On();
        else
            AutoOpen::Off();
    }

    void Write(wostream& out) const final
    {
        out << (AutoOpen::IsOn() ? PREF_ON : PREF_OFF);
    }
};

void Preferences::Initialize
(
    wstring const & wstrPrefFile,
    Sound         * pSound
)
{
    m_pSound = pSound;

    m_wstrPreferencesFile = current_path().wstring();
    m_wstrPreferencesFile += L"\\" + wstrPrefFile;

    Add<WrapSetColorMenu>(L"SetColorMenu");
    Add<WrapSetSound    >(L"SetSound"    );
    Add<WrapSetAutoOpen >(L"SetAutoOpen" );

    m_iNrOfToolboxEntries = m_prefVector.size();

    SymbolTable::ScrDefConst(PREF_OFF, 0L);
    SymbolTable::ScrDefConst(PREF_ON, 1L);
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
        wcout << Scanner::COMMENT_SYMBOL << L" +++ NNetPreferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Using defaults" << endl;
        return false;
    }
}

bool Preferences::WritePreferences() const
{
    wofstream prefFile(m_wstrPreferencesFile);
    prefFile << Scanner::COMMENT_START << L" User preferences" << endl;
    prefFile << Scanner::COMMENT_START << L" Toolbox " << endl;
    size_t i = 0;
    for (auto const& it : m_prefVector)
    {
        it->WriteCmdName(prefFile);
        it->Write(prefFile);
        prefFile << endl;
        if (++i == m_iNrOfToolboxEntries)
            prefFile << Scanner::COMMENT_START << L" Application" << endl;
    }
    prefFile << Scanner::COMMENT_START << L" End of Preferences" << endl;
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
