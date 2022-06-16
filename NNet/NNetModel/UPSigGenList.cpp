// UPSigGenList.cpp
//
// NNetModel

#include "stdafx.h"
#include "UPSigGenList.h"

using std::to_wstring;
using std::make_unique;
using std::ranges::find_if;

UPSigGenList::UPSigGenList()
{
    UPSigGen upSigGen { NewSigGen() };
    upSigGen->SetName(STD_SIG_GEN_NAME);
    PushSigGen(move(upSigGen));
    SetActive(SigGenId(0));
}

UPSigGenList::~UPSigGenList() = default;

SigGenId UPSigGenList::SetActive(SigGenId const id)
{
    SigGenId sigGenIdOld { m_sigGenIdActive };
    m_sigGenIdActive = id;
    return sigGenIdOld;
}

UPSigGen UPSigGenList::removeSigGen(vector<UPSigGen>::iterator it)
{
    if (it == m_list.end())
        return UPSigGen(nullptr);
    else
    {
        UPSigGen upSigGen = move(*it);
        if (upSigGen.get() == GetSigGenSelected() )
            SetActive(SigGenId(0));
        m_list.erase(it);
        return move(upSigGen);
    }
}

vector<UPSigGen>::iterator UPSigGenList::getSigGen(wstring const & name)
{
    return find_if(m_list, [&name](auto & it){ return it->GetName() == name; });
}

vector<UPSigGen>::const_iterator UPSigGenList::getSigGen(wstring const & name) const
{
    return find_if(m_list, [&name](auto & it){ return it->GetName() == name; });
}

vector<UPSigGen>::iterator UPSigGenList::getSigGen(SigGenId const id)
{
    return m_list.begin() + id.GetValue();
}

vector<UPSigGen>::const_iterator UPSigGenList::getSigGen(SigGenId const id) const
{
    return m_list.begin() + id.GetValue();
}

void UPSigGenList::InsertSigGen(UPSigGen upSigGen, SigGenId const id)
{
    m_list.insert(getSigGen(id), move(upSigGen));
}

SigGenId UPSigGenList::FindSigGen(wstring const & name) const
{
    auto it { getSigGen(name) };
    return SigGenId(Cast2Int(it - m_list.begin()));
}

SignalGenerator const * UPSigGenList::GetSigGen(SigGenId const id) const
{
    return m_list.at(id.GetValue()).get();
}

SignalGenerator * UPSigGenList::GetSigGen(SigGenId const id)
{
    return m_list.at(id.GetValue()).get();
}

SignalGenerator * UPSigGenList::GetSigGen(wstring const & name)
{
    auto it { getSigGen(name) };
    return (it == m_list.end()) ? nullptr : it->get();
}

SignalGenerator const * UPSigGenList::GetSigGen(wstring const & name) const
{
    auto it { getSigGen(name) };
    return (it == m_list.end()) ? nullptr : it->get();
}

bool UPSigGenList::IsInList(wstring const & name) const
{
    return FindSigGen(name).GetValue() < m_list.size();
}

UPSigGen UPSigGenList::NewSigGen()
{
    return move(make_unique<SignalGenerator>(GenerateUniqueName()));
}

UPSigGen UPSigGenList::NewSigGen(wstring const & name)
{
    return move(make_unique<SignalGenerator>(name));
}

SigGenId UPSigGenList::PushSigGen(UPSigGen upSigGen)
{
    m_list.push_back(move(upSigGen));
    return SigGenId(Cast2Int(m_list.size()-1));
}

UPSigGen UPSigGenList::PopSigGen()
{
    unique_ptr upSigGen { move(m_list.back()) };
    m_list.pop_back();
    return move(upSigGen);
}

UPSigGen UPSigGenList::RemoveSigGen(SigGenId const id)
{
    return move(removeSigGen(getSigGen(id)));
}

UPSigGen UPSigGenList::RemoveSigGen(wstring const & name)
{
    return move(removeSigGen(getSigGen(name)));  
}

UPSigGen UPSigGenList::RemoveSigGen()
{
    return move(RemoveSigGen(m_sigGenIdActive));  
}

wstring UPSigGenList::GenerateUniqueName() const
{
    int     iNr { 0 };
    wstring candidate;
    do
        candidate = L"SigGen " +  to_wstring(++iNr);
    while (GetSigGen(candidate) != nullptr);
    return candidate;
}
