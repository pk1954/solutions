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
import WrapBase;
import SoundInterface;

using std::wofstream;
using std::wostream;
using std::wstring;
using std::wcout;
using std::endl;
using std::make_unique;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::current_path;

void Preferences::Initialize(wstring const & wstrPrefFile)
{
    m_wstrPreferencesFile = current_path().wstring();
    m_wstrPreferencesFile += L"\\" + wstrPrefFile;

    WrapBaseBool::Initialize();

    AddBoolWrapper(L"SetSound",     m_bSound);
    AddBoolWrapper(L"SetColorMenu", m_bColorMenu);
    AddBoolWrapper(L"SetAutoOpen",  m_bAutoOpen);
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
        wcout << Scanner::COMMENT_SYMBOL << L" +++ NNetPreferences file " << m_wstrPreferencesFile << L" not found" << endl;
        wcout << Scanner::COMMENT_SYMBOL << L" +++ Using defaults" << endl;
        return false;
    }
}

bool Preferences::WritePreferences()
{
    wofstream prefFile(m_wstrPreferencesFile);
    prefFile << Scanner::COMMENT_START << L" User preferences" << endl;
    size_t i = 0;
    for (auto const& it : m_prefVector)
    {
        it->Write(prefFile);
        prefFile << endl;
    }
    prefFile << Scanner::COMMENT_START << L" End of Preferences" << endl;
    prefFile.close();
    wcout << Scanner::COMMENT_START << L"preferences file " << m_wstrPreferencesFile << L" written" << endl;
    return true;
}
