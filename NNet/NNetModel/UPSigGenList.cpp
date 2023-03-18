// UPSigGenList.cpp
//
// NNetModel

module;

#include <cassert>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

module NNetModel:UPSigGenList;

import :SigGenId;
import :StdSigGen;
import :SignalGenerator;
import :InputLine;

using std::wstring;
using std::vector;
using std::to_wstring;
using std::make_unique;
using std::unique_ptr;
using std::ranges::find_if;
using std::ranges::for_each;

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
        if (it->get() == GetSigGenSelected())
            SetActive(STD_SIGGEN);
        UPSigGen upSigGen = move(*it);
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
    assert(id != STD_SIGGEN);
    return m_list.begin() + id.GetValue();
}

vector<UPSigGen>::const_iterator UPSigGenList::getSigGen(SigGenId const id) const
{
    assert(id != STD_SIGGEN);
    return m_list.begin() + id.GetValue();
}

void UPSigGenList::InsertSigGen(UPSigGen upSigGen, SigGenId const id)
{
    assert(id != STD_SIGGEN);
    m_list.insert(getSigGen(id), move(upSigGen));
}

SigGenId UPSigGenList::FindSigGen(wstring const & name) const
{
    if (name == StdSigGen::Get()->GetName())
        return STD_SIGGEN;
    auto it { getSigGen(name) };
    if (it != m_list.end())
         return SigGenId(Cast2Int(it - m_list.begin()));
    else 
       return INVALID_SIGGEN;
}

SignalGenerator const * UPSigGenList::GetSigGen(SigGenId const id) const
{
    return (id == STD_SIGGEN)
           ? StdSigGen::Get()
           : m_list.at(id.GetValue()).get();
}

SignalGenerator * UPSigGenList::GetSigGen(SigGenId const id)
{
    return (id == STD_SIGGEN)
        ? StdSigGen::Get()
        : m_list.at(id.GetValue()).get();
}

SignalGenerator * UPSigGenList::GetSigGen(wstring const & name)
{
    auto it { getSigGen(name) };
    return (it == m_list.end()) ? nullptr : it->get();
}

SignalGenerator const * UPSigGenList::GetSigGen(wstring const & name) const
{
    if (name == StdSigGen::Get()->GetName())
        return StdSigGen::Get();
    auto it { getSigGen(name) };
    return (it == m_list.end()) ? nullptr : it->get();
}

bool UPSigGenList::IsInList(wstring const & name) const
{
    SigGenId id { FindSigGen(name) };
    return IsValid(id) && (id != STD_SIGGEN);
}

UPSigGen UPSigGenList::NewSigGen()
{
    return make_unique<SignalGenerator>(GenerateUniqueName());
}

UPSigGen UPSigGenList::NewSigGen(wstring const & name)
{
    return make_unique<SignalGenerator>(name);
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
    assert(id != STD_SIGGEN);
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

SigGenId UPSigGenList::GetSigGenId(SignalGenerator const& sigGen) const
{
    if (&sigGen == StdSigGen::Get())
        return STD_SIGGEN;
    SigGenId sigGenFound { };
    SigGenId id { SigGenId(0) };
    for_each
    (
        m_list,
        [&sigGen, &sigGenFound, &id](auto& up)
        {
            if (&sigGen == up.get())
                sigGenFound = id;
            ++id;
        }
    );
    assert(IsValid(sigGenFound)); 
    return sigGenFound;
}

SigGenId UPSigGenList::GetSigGenId(fPixelPoint const &fPixCrsr) const
{
    fPixel fPixMaxX { DIST * Cast2Float(m_list.size() + 1) - GAP };
    if (fPixCrsr.GetX() > fPixMaxX)
        return INVALID_SIGGEN;
    if (fPixCrsr.GetY() > SignalGenerator::SIGGEN_HEIGHT)
        return INVALID_SIGGEN;
    SigGenId id { Cast2Int(fPixCrsr.GetX() / SignalGenerator::SIGGEN_WIDTH) };
    return id;
}

void UPSigGenList::DrawSignalGenerators(D2D_driver& graphics) const
{
    fPixelRect fPixRect { 1._fPixel, 1._fPixel, SignalGenerator::SIGGEN_WIDTH, SignalGenerator::SIGGEN_HEIGHT };
    Apply2AllC
    (
        [this, &fPixRect, &graphics](auto const& pSigGen)
        {
            pSigGen->DrawSigGen(graphics, fPixRect, IsSelected(*pSigGen));
            fPixRect.MoveHorz(DIST);
        }
    );
}

void UPSigGenList::DrawInputCable
(
    D2D_driver                 & graphics,
    Uniform2D<MicroMeter> const& coord,
    fPixel                const  fPixPos,
    InputLine             const& inputLine,
    ID2D1SolidColorBrush* const  pBrush
) const
{
    MicroMeterPnt const umDirVector      { inputLine.GetDirVector() };
    MicroMeterPnt const umCenter         { inputLine.GetPos() - umDirVector * 0.7f };
    fPixelPoint   const fPixPosInputLine { coord.Transform2fPixelPos(umCenter) };
    fPixelPoint   const fPixPosDir       { coord.Transform2fPixelSize(umDirVector) };
    fPixelPoint   const fPixPosSigGen    { fPixPos, SignalGenerator::SIGGEN_HEIGHT };

    graphics.DrawBezier
    (
        fPixPosSigGen,
        fPixPosSigGen + fPixelPoint(0.0_fPixel, 100.0_fPixel),
        fPixPosInputLine - fPixPosDir.ScaledTo(100.0_fPixel),
        fPixPosInputLine,
        pBrush,
        2._fPixel
    );
}
