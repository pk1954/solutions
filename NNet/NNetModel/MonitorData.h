// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "Signal.h"
#include "Track.h"
#include "SignalId.h"

using std::vector;

class MonitorData
{
public:
	using TrackNrFunc = function<void(TrackNr const)>;

	MonitorData()                              = default;  // constructor   
	~MonitorData()                             = default;  // destructor
	MonitorData(MonitorData&& rhs)             = delete;   // move constructor
	MonitorData& operator=(const MonitorData&) = delete;   // copy assignment operator
	
	MonitorData(const MonitorData&);                       // copy constructor
	MonitorData& operator=(MonitorData&&) noexcept;        // move assignment operator

	bool operator== (MonitorData const &) const;

	void CheckTracks() const;  // for debugging

	void Reset();

	int  GetNrOfTracks() const;
	bool NoTracks     () const;

	bool const IsValid(TrackNr const trackNr) const;
	bool const IsValid(SignalId const &) const;

	void InsertTrack(TrackNr const);
	void DeleteTrack(TrackNr const);

	void SetHighSigObservable(Observable * pObs) { m_pHighSigObservable = pObs; }

	SignalId const SetHighlightedSignal(MicroMeterPnt const &);
	SignalId const SetHighlightedSignal(Signal        const &);
	SignalId const SetHighlightedSignal(SignalId      const  );
	SignalId const ResetHighlightedSignal();

	SignalNr       const AddSignal            (TrackNr  const, MicroMeterCircle const &);
	SignalNr       const AddSignal            (TrackNr  const, unique_ptr<Signal>);
	void                 AddSignal            (SignalId const &, unique_ptr<Signal>);
	SignalNr       const MoveSignal           (SignalId const &, TrackNr  const);
	void                 MoveSignal           (SignalId const &, SignalId const &);
	Signal const * const GetSignalPtr         (SignalId const &) const;
	Signal       * const GetSignalPtr         (SignalId const &);
	Signal       * const FindSignal           (Signal::Crit    const &) const;
	SignalId       const FindSignalId         (Signal::Crit    const &) const;
	Signal       * const FindSensor           (MicroMeterPnt const &) const;
	Signal const * const GetHighlightedSignal () const;
	Signal       * const GetHighlightedSignal ();

	unique_ptr<Signal> DeleteSignal(SignalId const &);

	void Apply2AllTracks        (TrackNrFunc const &) const;
	void Apply2AllSignalsInTrack(TrackNr const, SignalNrFunc const &) const;
	void Apply2AllSignals       (SignalId::Func const &) const;
	void Apply2AllSignals       (Signal::Func   const &) const;

	SignalId const GetHighlightedSignalId()       const { return m_idSigHighlighted; }
	TrackNr  const GetSelectedTrackNr ()          const { return m_idSigHighlighted.GetTrackNr(); }
	bool     const IsAnySignalSelected()          const { return m_idSigHighlighted.IsNotNull(); }
	bool     const IsSelected(SignalId const &id) const { return m_idSigHighlighted == id; }
	bool     const IsEmptyTrack(TrackNr const)    const;


private:
	Track       * const getTrack(TrackNr const);
	Track const * const getTrack(TrackNr const) const;
	unique_ptr<Signal>  removeSignal(SignalId const &);

	Observable              * m_pHighSigObservable { nullptr };
	SignalId                  m_idSigHighlighted   {};
	vector<unique_ptr<Track>> m_tracks             {};
};
