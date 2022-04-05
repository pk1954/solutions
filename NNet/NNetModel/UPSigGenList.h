// UPSigGenList.h 
//
// NNetModel

#pragma once

#include "NamedType.h"

class SignalGenerator;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find_if;
using std::ranges::for_each;

using SigGenId = NamedType<size_t, struct SigGenIdParam>;
using UPSigGen = unique_ptr<SignalGenerator>;

class UPSigGenList
{
public:
    UPSigGenList();

    SignalGenerator       * StdSigGen() { return m_list.begin()->get(); }
    SigGenId                FindSigGen         (wstring  const &) const;
    SignalGenerator const * GetSigGen (SigGenId const  ) const;
    SignalGenerator       * GetSigGen (SigGenId const  );
    SignalGenerator       * GetSigGen (wstring  const &);
    SignalGenerator const * GetSigGen (wstring  const &) const;
    bool                    IsInList           (wstring  const &) const;
    bool                    IsValid(SigGenId const id) const { return id.GetValue() < m_list.size(); }

    UPSigGen NewSigGen();
    UPSigGen NewSigGen(wstring  const &);
    SigGenId PushSigGen(UPSigGen);
    UPSigGen PopSigGen();
    UPSigGen RemoveSigGen(SigGenId const);
    UPSigGen RemoveSigGen(wstring const &);
    wstring  GenerateUniqueName() const;
    size_t   Size() { return m_list.size(); }

    void Apply2All(auto const &f) const 
    { 
        for_each(m_list, [&f](auto const & up) { f(up.get()); });
    }

private:

    vector<UPSigGen> m_list;

    vector<UPSigGen>::      iterator getSigGen(wstring const &);
    vector<UPSigGen>::const_iterator getSigGen(wstring const &) const;

    UPSigGen removeSigGen(vector<UPSigGen>::iterator);
};
