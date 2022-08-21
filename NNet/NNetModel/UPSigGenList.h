// UPSigGenList.h 
//
// NNetModel

#pragma once

#include <vector>
#include "SigGenId.h"
#include "SignalGenerator.h"

import NamedType;

using std::ranges::for_each;
using std::unique_ptr;
using std::wstring;
using std::vector;

using UPSigGen = unique_ptr<SignalGenerator>;

class UPSigGenList
{
public:
    UPSigGenList();
    ~UPSigGenList();

    size_t Size() const { return m_list.size(); }

    SignalGenerator const * GetSigGen(SigGenId const) const;
    SignalGenerator       * GetSigGen(SigGenId const);

    SigGenId                GetSigGenIdSelected() const { return m_sigGenIdActive; }
    SignalGenerator const * GetSigGenSelected  () const { return GetSigGen(m_sigGenIdActive); }
    SignalGenerator       * GetSigGenSelected  ()       { return GetSigGen(m_sigGenIdActive); }

    bool IsAnySigGenSelected()      const { return m_sigGenIdActive.IsNotNull(); }
    bool IsValid(SigGenId const id) const { return id.GetValue() < m_list.size(); }

    SigGenId SetActive(SigGenId const);
    SigGenId PushSigGen(UPSigGen);
    UPSigGen PopSigGen();
    UPSigGen RemoveSigGen(SigGenId const);
    UPSigGen RemoveSigGen();
    void     InsertSigGen(UPSigGen, SigGenId const);

    void Apply2All(auto const &f)  
    { 
        for_each(m_list, [&f](auto & up) { f(up.get()); });
    }

    void Apply2AllC(auto const &f) const
    { 
        for_each(m_list, [&f](auto const & up) { f(up.get()); });
    }

    SignalGenerator       * StdSigGen() { return m_list.begin()->get(); }

    UPSigGen                NewSigGen();
    SignalGenerator       * GetSigGen   (wstring const &);
    SignalGenerator const * GetSigGen   (wstring const &) const;
    bool                    IsInList    (wstring const &) const;
    SigGenId                FindSigGen  (wstring const &) const;
    UPSigGen                NewSigGen   (wstring const &);
    UPSigGen                RemoveSigGen(wstring const &);
    wstring                 GenerateUniqueName() const;

private:

    inline static wstring STD_SIG_GEN_NAME { L"Standard" };

    vector<UPSigGen> m_list;
    SigGenId         m_sigGenIdActive { 0 };

    vector<UPSigGen>::      iterator getSigGen(wstring const &);
    vector<UPSigGen>::const_iterator getSigGen(wstring const &) const;

    vector<UPSigGen>::      iterator getSigGen(SigGenId const);
    vector<UPSigGen>::const_iterator getSigGen(SigGenId const) const;

    UPSigGen removeSigGen(vector<UPSigGen>::iterator);
};
