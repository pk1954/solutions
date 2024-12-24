// BoolPreferences.ixx
//
// Win32_utilities

export module BoolPreferences;

import std;
import Types;
import IoUtil;
import BoolType;
import OnOffPair;
import ObserverInterface;
import Win32_Util_Resource;
import WinBasics;

using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;

export using IdBoolPref = NamedType<size_t, struct IdBoolPref_Parameter>;

export class BoolPreferences
{
public:

    static IdBoolPref CreateNew(wstring const& wstrExternalName)
    {
        IdBoolPref const idResult(m_list.size());
        m_list.emplace_back(make_unique<BOOL_PREF>());
        Preferences::AddBoolWrapper(wstrExternalName, get(idResult).m_bFlag);
        return idResult;
    }

    static bool IsActive(IdBoolPref const id) 
    { 
        return get(id).m_bFlag.Get(); 
    }

    static void RegisterObserver(ObserverInterface& observer)
    {
        for (auto &up: m_list)
            up->m_bFlag.RegisterObserver(observer);
    }

    static void Toggle(int const iCmd)
    {
        get(IdBoolPref(iCmd - IDM_BIN_SWITCH_START)).m_bFlag.Toggle();
    }

    static void AppendOnOffMenu
    (
        IdBoolPref const id,
        HMENU            hMenu, 
        LPCTSTR    const title
    )
    {
        get(id).m_onOffPair.AppendOnOffMenu(hMenu, title);
    }

    static void EnableOnOff(HMENU hMenu)
    {
        for (auto &up: m_list)
            up->EnableOnOff(hMenu);
    }

private:

    inline static int m_iNextCmdId { IDM_BIN_SWITCH_START };

    struct BOOL_PREF
    {
        OnOffPair m_onOffPair;
        BoolType  m_bFlag;

        BOOL_PREF()
          : m_onOffPair(m_iNextCmdId++),
            m_bFlag(true)
        {}

        void EnableOnOff(HMENU hMenu)
        {
            m_onOffPair.EnableOnOff(hMenu, m_bFlag.Get());
        }
    };

    inline static vector<unique_ptr<BOOL_PREF>> m_list;

    static BOOL_PREF &get(IdBoolPref const id) 
    { 
        return *m_list.at(id.GetValue()); 
    }
};