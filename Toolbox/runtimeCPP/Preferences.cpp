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

class WrapSetColorMenu : public WrapBase
{
public:
    WrapSetColorMenu
    (
        wstring const & wstrName,
        Preferences   & pref
    )
      : WrapBase(wstrName),
        m_pref(pref)
    {}

    void operator() (Script& script) const final
    {
        m_pref.SetColorMenu(script.ScrReadBool());
    }

    void Write(wostream& out) const final
    {
        out << (m_pref.ColorMenuVisible() ? PREF_ON : PREF_OFF);
    }

public:
    Preferences& m_pref;
};

void Preferences::Initialize(wstring const & wstrPrefFile)
{
    m_wstrPreferencesFile = current_path().wstring();
    m_wstrPreferencesFile += L"\\" + wstrPrefFile;

    AddWrapper(make_unique<WrapSetColorMenu>(L"SetColorMenu", *this));

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
    for (auto const& it : m_prefVector)
    {
        it->WriteCmdName(prefFile);
        it->Write(prefFile);
        prefFile << endl;
    }
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
