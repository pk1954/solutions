// UPSigGenList.h 
//
// NNetModel

#pragma once

#include "NamedType.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find_if;
using std::ranges::for_each;

using SigGenId = NamedType<size_t, struct SigGenIdParam>;

class UPSigGenList
{
private:

   auto getSigGen(wstring const & name)
    {
        return find_if(m_list, [&name](auto & it){ return it->GetName() == name; });
    }

    auto getSigGen(wstring const & name) const
    {
        return find_if(m_list, [&name](auto & it){ return it->GetName() == name; });
    }

public:

    void SetParameters(Param * pParam)
    {
        m_pParameters = pParam;
    }

    Param const & GetParametersC() const { return * m_pParameters;}
    Param       & GetParameters ()       { return * m_pParameters;}

    SigGenId FindSignalGenerator(wstring const & name) const
    {
        auto it { getSigGen(name) };
        return static_cast<SigGenId>(it - m_list.end());
    }

    SignalGenerator const * GetSignalGenerator(SigGenId const id) const
    {
        return m_list.at(id.GetValue()).get();
    }

    SignalGenerator * GetSignalGenerator(wstring const & name)
    {
        auto it { getSigGen(name) };
        return (it == m_list.end()) ? nullptr : it->get();
    }

    SignalGenerator const * GetSignalGenerator(wstring const & name) const
    {
        auto it { getSigGen(name) };
        return (it == m_list.end()) ? nullptr : it->get();
    }

    UPSigGen NewSigGen()
    {
        return move(make_unique<SignalGenerator>(*this));
    }

    SigGenId PushSigGen(UPSigGen upSigGen)
    {
        m_list.push_back(move(upSigGen));
        return static_cast<SigGenId>(m_list.size()-1);
    }

    UPSigGen PopSigGen()
    {
        unique_ptr upSigGen { move(m_list.back()) };
        m_list.pop_back();
        return move(upSigGen);
    }

    UPSigGen RemoveSigGen(SigGenId const id)
    {
        return move(removeSigGen(m_list.begin() + id.GetValue()));
    }

    UPSigGen RemoveSigGen(wstring const & name)
    {
        return move(removeSigGen(getSigGen(name)));
    }

    void Apply2All(auto const &f) const 
    { 
        for_each(m_list, [&f](auto const & up) { f(up.get()); });
    }

    wstring GenerateUniqueName() const
    {
        int     iNr { 0 };
        wstring candidate;
        do
            candidate = L"SigGen " +  to_wstring(++iNr);
        while (GetSignalGenerator(candidate) != nullptr);
        return candidate;
    }

private:
    vector<UPSigGen> m_list;

    Param * m_pParameters;

    UPSigGen removeSigGen(vector<UPSigGen>::iterator it)
    {
        if (it == m_list.end())
            return UPSigGen(nullptr);
        else
        {
            UPSigGen upSigGen = move(*it);
            m_list.erase(it);
            return move(upSigGen);
        }
    }
};
