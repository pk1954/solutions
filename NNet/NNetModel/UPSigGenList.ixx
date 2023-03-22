// UPSigGenList.ixx
//
// NNetModel

module;

#include <string>
#include <vector>
#include <algorithm>
#include <memory>

export module NNetModel:UPSigGenList;

import Observable;
import Types;
import :SigGenId;
import :StdSigGen;
import :SignalGenerator;
import :InputLine;

using std::ranges::for_each;
using std::unique_ptr;
using std::wstring;
using std::vector;

export using UPSigGen = unique_ptr<SignalGenerator>;

export class UPSigGenList
{
public:
    size_t Size() const { return m_list.size(); }

    SignalGenerator const * GetSigGen(SigGenId const) const;
    SignalGenerator       * GetSigGen(SigGenId const);

    SigGenId                GetSigGenId(SignalGenerator const&) const;
    SigGenId                GetSigGenId(fPixelPoint     const&) const;

    SigGenId                GetSigGenIdSelected() const { return m_sigGenIdActive; }
    SignalGenerator const * GetSigGenSelected  () const { return GetSigGen(m_sigGenIdActive); }
    SignalGenerator       * GetSigGenSelected  ()       { return GetSigGen(m_sigGenIdActive); }

    bool IsInNewSigGenButton(fPixelPoint const& fPixPnt) const { return newSigGenButtonRect().Includes(fPixPnt); }

    bool IsValid(SigGenId const id)           const { return (STD_SIGGEN == id) || (id.GetValue() < m_list.size()); }
    bool IsAnySigGenSelected()                const { return m_sigGenIdActive.IsNotNull(); }
    bool IsSelected(SigGenId const id)        const { return id == m_sigGenIdActive; }
    bool IsSelected(SignalGenerator const &s) const { return &s == GetSigGenSelected(); }

    void     SetName(SigGenId const, wstring const&);
    SigGenId SetActive(SigGenId const);
    SigGenId PushSigGen(UPSigGen);
    UPSigGen PopSigGen();
    UPSigGen RemoveSigGen(SigGenId const);
    UPSigGen RemoveSigGen();
    void     InsertSigGen(UPSigGen, SigGenId const);
    void     DrawSignalGenerators(D2D_driver&) const;
    void     DrawNewSigGenButton (D2D_driver&) const;
    void     DrawInputCable
    (
        D2D_driver&,
        Uniform2D<MicroMeter> const&,
        fPixel const,
        InputLine const&,
        ID2D1SolidColorBrush* const
    ) const;

    void Apply2All(auto const& f)
    {
        f(StdSigGen::Get());
        for_each(m_list, [&f](auto& up) { f(up.get()); });
    }

    void Apply2AllC(auto const& f) const
    {
        f(StdSigGen::Get());
        for_each(m_list, [&f](auto const& up) { f(up.get()); });
    }

    UPSigGen                NewSigGen();
    SignalGenerator       * GetSigGen   (wstring const &);
    SignalGenerator const * GetSigGen   (wstring const &) const;
    bool                    IsInList    (wstring const &) const;
    SigGenId                FindSigGen  (wstring const &) const;
    UPSigGen                NewSigGen   (wstring const &);
    UPSigGen                RemoveSigGen(wstring const &);
    wstring                 GenerateUniqueName() const;

    void SetActiveSigGenObservable(Observable &o) { m_pActiveSigGenObservable = &o; }

private:
    inline static const fPixel GAP  { 2._fPixel };
    inline static const fPixel DIST { SignalGenerator::SIGGEN_WIDTH + GAP };

    fPixel areaWidth() const { return DIST * Cast2Float(m_list.size() + 1); }
    fPixelRect newSigGenButtonRect() const;

    vector<UPSigGen> m_list;  // std siggen is ** not ** in list!
    SigGenId         m_sigGenIdActive { STD_SIGGEN };
    Observable *     m_pActiveSigGenObservable { nullptr };

    vector<UPSigGen>::iterator       getSigGen(wstring const &);
    vector<UPSigGen>::const_iterator getSigGen(wstring const &) const;

    vector<UPSigGen>::iterator       getSigGen(SigGenId const);
    vector<UPSigGen>::const_iterator getSigGen(SigGenId const) const;

    UPSigGen removeSigGen(vector<UPSigGen>::iterator);
};
