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

    static IdBoolPref CreateNew
    (
        wstring const& wstrExternalName,
        HMENU   const  hMenu,
        LPCTSTR const  menuName
    )
    {
        unique_ptr<BOOL_PREF> upBoolPref { make_unique<BOOL_PREF>() };       // create BOOL_PREF
        upBoolPref->m_onOffPair.AppendOnOffMenu(hMenu, menuName);            // Append to menu
        Preferences::AddBoolWrapper(wstrExternalName, upBoolPref->m_bFlag);  // Register for reference I/O
        m_list.push_back(move(upBoolPref));                                  // Append to list
        return IdBoolPref(m_list.size() - 1);                                // return index in list
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

    static void EnableOnOff(HMENU hMenu)
    {
        for (auto &up: m_list)
            up->m_onOffPair.EnableOnOff(hMenu, up->m_bFlag.Get());
    }

private:

    inline static int m_iNextCmdId { IDM_BIN_SWITCH_START };

    struct BOOL_PREF
    {
        BOOL_PREF()
          : m_onOffPair(m_iNextCmdId++),  // generate command id
            m_bFlag(false)
        {}

        OnOffPair m_onOffPair;
        BoolType  m_bFlag;
    };

    inline static vector<unique_ptr<BOOL_PREF>> m_list;

    static BOOL_PREF &get(IdBoolPref const id) 
    { 
        return *m_list.at(id.GetValue()); 
    }
};