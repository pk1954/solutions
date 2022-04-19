// UPSigGenList.h 
//
// NNetModel

#pragma once

#include "Observable.h"
#include "NamedType.h"

using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find_if;
using std::ranges::for_each;

class SignalGenerator;

using SigGenId = NamedType<size_t, struct SigGenIdParam>;
using UPSigGen = unique_ptr<SignalGenerator>;

class UPSigGenList //: public Observable
{
public:
    UPSigGenList();

    SignalGenerator       * StdSigGen() { return m_list.begin()->get(); }

    SigGenId                FindSigGen(wstring  const &) const;
    SignalGenerator const * GetSigGen (SigGenId const  ) const;
    SignalGenerator       * GetSigGen (SigGenId const  );
    SignalGenerator       * GetSigGen (wstring  const &);
    SignalGenerator const * GetSigGen (wstring  const &) const;
    bool                    IsInList  (wstring  const &) const;

    SigGenId                GetSigGenIdActive() const { return m_sigGenIdActive; }
    SignalGenerator const * GetSigGenActive  () const { return GetSigGen(m_sigGenIdActive); }
    SignalGenerator       * GetSigGenActive  ()       { return GetSigGen(m_sigGenIdActive); }

    bool IsValid(SigGenId const id) const { return id.GetValue() < m_list.size(); }

    UPSigGen NewSigGen();
    UPSigGen NewSigGen(wstring  const &);
    SigGenId PushSigGen(UPSigGen);
    UPSigGen PopSigGen();
    UPSigGen RemoveSigGen(SigGenId const);
    UPSigGen RemoveSigGen(wstring const &);
    UPSigGen RemoveSigGen();
    void     InsertSigGen(UPSigGen, SigGenId const);
    wstring  GenerateUniqueName() const;
    size_t   Size() { return m_list.size(); }

    void Apply2All(auto const &f) const 
    { 
        for_each(m_list, [&f](auto const & up) { f(up.get()); });
    }

    SigGenId SetActive(SigGenId const id)
    {
        SigGenId sigGenIdOld { m_sigGenIdActive };
        m_sigGenIdActive = id;
        //NotifyAll(false);
        return sigGenIdOld;
    }

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
