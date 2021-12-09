// MonitorData.h : 
//
// NNetModel

#pragma once

#include <exception>
#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "Signal.h"
#include "Track.h"
#include "SignalId.h"

using std::vector;
using std::exception;

class MonitorData;

struct MonitorDataException: public exception
{
	MonitorDataException
	(
		MonitorData const & data,
		TrackNr     const   trackNr,
		wstring     const & msg
	)
	  : m_data(data),
		m_trackNr(trackNr),
		m_wstrMessage(msg)
	{}
	MonitorData const & m_data;
	TrackNr     const   m_trackNr;
	wstring     const   m_wstrMessage;
};

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

	void Dump() const;
	void CheckTracks() const;  // for debugging

	void Reset();

	int  GetNrOfTracks() const;
	bool NoTracks     () const;

	bool IsValid(TrackNr const trackNr) const;
	bool IsValid(SignalId const &) const;

	void InsertTrack(TrackNr const);
	void DeleteTrack(TrackNr const);

	void SetHighSigObservable(Observable * pObs) { m_pHighSigObservable = pObs; }

	SignalId SetHighlightedSignal(MicroMeterPnt const &);
	SignalId SetHighlightedSignal(Signal        const &);
	SignalId SetHighlightedSignal(SignalId      const  );
	SignalId ResetHighlightedSignal();

	SignalNr       AddSignal            (TrackNr  const,   MicroMeterCircle const &);
	SignalNr       AddSignal            (TrackNr  const,   unique_ptr<Signal>);
	void           AddSignal            (SignalId const &, unique_ptr<Signal>);
	SignalNr       MoveSignal           (SignalId const &, TrackNr  const);
	Signal       * GetSignalPtr         (SignalId const &);
	Signal const * GetSignalPtr         (SignalId const &)      const;
	Signal       * FindSignal           (Signal::Crit  const &) const;
	SignalId       FindSignalId         (Signal::Crit  const &) const;
	Signal       * FindSensor           (MicroMeterPnt const &) const;
	Signal const * GetHighlightedSignal () const;
	Signal       * GetHighlightedSignal ();

	unique_ptr<Signal> DeleteSignal(SignalId const &);

	void     Apply2AllTracks        (TrackNrFunc const &)                 const;
	void     Apply2AllSignalsInTrack(TrackNr const, SignalNrFunc const &) const;
	void     Apply2AllSignals       (SignalId::Func const &)              const;
	void     Apply2AllSignals       (Signal::Func   const &)              const;

	SignalId GetHighlightedSignalId()       const { return m_idSigHighlighted; }
	TrackNr  GetSelectedTrackNr ()          const { return m_idSigHighlighted.GetTrackNr(); }
	bool     IsAnySignalSelected()          const { return m_idSigHighlighted.IsNotNull(); }
	bool     IsSelected(SignalId const &id) const { return m_idSigHighlighted == id; }
	bool     IsEmptyTrack(TrackNr const)    const;

	static void HandleException(MonitorDataException const &);

private:
	Track            * getTrack(TrackNr const);
	Track      const * getTrack(TrackNr const) const;
	unique_ptr<Signal> removeSignal(SignalId const &);

	Observable              * m_pHighSigObservable { nullptr };
	SignalId                  m_idSigHighlighted   {};
	vector<unique_ptr<Track>> m_tracks             {};
};
