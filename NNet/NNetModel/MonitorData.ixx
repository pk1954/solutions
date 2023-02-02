// MonitorData.ixx
//
// NNetModel

module;

#include <vector>
#include <functional>
#include <algorithm>

export module NNetModel:MonitorData;

import Observable;
import Types;
import :SignalId;
import :Signal;
import :TrackNr;
import :Track;

using std::function;
using std::vector;
using std::unique_ptr;
using std::ranges::any_of;

export class MonitorData
{
public:
	using TrackNrFunc = function<void(TrackNr const)>;

	MonitorData()  = default;  // constructor   
	~MonitorData() = default;  // destructor
	MonitorData            (      MonitorData &&) = delete;   // move constructor
	MonitorData & operator=(const MonitorData &)  = delete;   // copy assignment operator
	MonitorData            (const MonitorData &)  = delete;   // copy constructor
	MonitorData & operator=(      MonitorData &&) noexcept;   // move assignment operator

//	bool operator== (MonitorData const &) const;
// TODO
	void Dump() const;
	void CheckTracks() const;  // for debugging

	void Reset();
	void ClearDynamicData();

	int  GetNrOfTracks() const;
	bool NoTracks() const;

	bool IsValid(TrackNr const trackNr) const;
	bool IsValid(SignalId const &) const;

	void InsertTrack(TrackNr const);
	void DeleteTrack(TrackNr const);

	void SetHighSigObservable(Observable * pObs) { m_pHighSigObservable = pObs; }

	//SignalId SetHighlightedSignal(MicroMeterPnt const &);
	SignalId SetHighlightedSignal(Signal        const &);
	SignalId SetHighlightedSignal(SignalId      const);
	SignalId ResetHighlightedSignal();

	void           AddStimulus      (fMicroSecs    const);
	SignalNr       AddSignal        (TrackNr       const,   unique_ptr<Signal>);
	void           AddSignal        (SignalId      const &, unique_ptr<Signal>);
	SignalNr       MoveSignal       (SignalId      const &, TrackNr const);
	Signal       * GetSignalPtr     (SignalId      const &);
	Signal const * GetConstSignalPtr(SignalId      const &) const;
//	SignalId       FindSignalId     (MicroMeterPnt const &) const;
	Signal const * GetHighlightedSignal() const;

	unique_ptr<Signal> DeleteSignal(SignalId const&);

	Signal const * FindSignal(auto const& crit) const
	{
		for (unique_ptr<Track> const& upTrack : m_tracks)
			if (Signal const * const pSignal{ upTrack->FindSignal(crit) })
				return pSignal;
		return nullptr;
	}

	SignalId FindSignalId(auto const & crit) const
	{
		for (int i = 0; i < m_tracks.size(); ++i)
		{
			TrackNr  const trackNr  { TrackNr(i) };
			SignalNr const signalNr { getTrack(trackNr)->FindSignalNr(crit) };
			if (signalNr.IsNotNull())
				return SignalId(trackNr, signalNr);
		}
		return SignalId::NULL_VAL();
	}

	void Apply2AllTracksC(auto const & func) const
	{
		for (auto trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
			func(trackNr);
	}

	void Apply2AllTracksRevC(auto const& func) const
	{
		for (auto trackNr = TrackNr(GetNrOfTracks() - 1); trackNr >= TrackNr(0); --trackNr)
			func(trackNr);
	}

	bool Apply2AllTracksB(auto const& func) const
	{
		return any_of(m_tracks, [&func](auto const& up) { return func(*up.get()); });
	}

	void Apply2AllSignalIdsC(auto const& func) const
	{
		for (auto trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
		{
			if (Track const * pTrack{ getTrack(trackNr) })
				pTrack->Apply2AllSignalNrsC
				(
					[&func, &trackNr](SignalNr const& signalNr) { func(SignalId(trackNr, signalNr)); }
			    );
		}
	}

	void Apply2AllSignals(auto const& func)
	{
		for (auto trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
		{
			if (Track * pTrack{ getTrack(trackNr) })
				pTrack->Apply2AllSignals([&func](Signal & signal) { func(signal); });
		}
	}

	void Apply2AllSignalsC(auto const& func) const
	{
		for (auto trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
		{
			if (Track const * pTrack{ getTrack(trackNr) })
				pTrack->Apply2AllSignalsC([&func](Signal const & signal) { func(signal); });
		}
	}

	void Apply2AllSignalsInTrackC(TrackNr const trackNr, auto const& func) const
	{
		if (IsValid(trackNr))
			getTrack(trackNr)->Apply2AllSignalNrsC(func);
	}

	int      GetNrOfSignals()               const { return m_iNrOfSignals; }
	SignalId GetHighlightedSignalId()       const { return m_idSigHighlighted; }
	TrackNr  GetSelectedTrackNr()           const { return m_idSigHighlighted.GetTrackNr(); }
	bool     IsAnySignalSelected()          const { return m_idSigHighlighted.IsValid(); }
	bool     IsSelected(SignalId const& id) const { return m_idSigHighlighted == id; }
	bool     IsEmptyTrack(TrackNr const)    const;
	bool     AnyEmptyTracks()               const;

	vector<fMicroSecs> const& GetStimulusList() const { return m_usStimulusList; }

private:
	Track            * getTrack(TrackNr const);
	Track      const * getTrack(TrackNr const) const;
	unique_ptr<Signal> removeSignal(SignalId const&);

	Observable              * m_pHighSigObservable { nullptr };
	SignalId                  m_idSigHighlighted   { };
	vector<unique_ptr<Track>> m_tracks             { };
	int                       m_iNrOfSignals       { 0 };
	vector<fMicroSecs>        m_usStimulusList;
};
