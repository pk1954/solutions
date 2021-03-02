// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "NamedType.h"
#include "Signal.h"
#include "Track.h"
#include "SignalId.h"

using std::vector;

using TrackNrFunc = function<void(TrackNr const)>;

class MonitorData
{
public:
	MonitorData()                                = default;  // constructor   
	~MonitorData()                               = default;  // destructor
	MonitorData( MonitorData&& rhs )             = delete;   // move constructor
	MonitorData& operator=( const MonitorData& ) = delete;   // copy assignment operator
	
	MonitorData(const MonitorData& rhs)  // copy constructor
	{
		for ( auto const & upTrack : rhs.m_tracks )
			m_tracks.push_back( move(make_unique<Track>(*upTrack.get())) );
	}

	MonitorData& operator=(MonitorData&& rhs)  // move assignment operator
	{
		if (this != &rhs)
		{
			m_tracks.clear();
			for ( auto const & upTrack : rhs.m_tracks )
				m_tracks.push_back( move(make_unique<Track>(*upTrack.get())) );
		}
		return * this;
	}

	bool operator== (MonitorData const & ) const;

	void CheckTracks() const;  // for debugging

	void Reset();

	int  GetNrOfTracks() const;
	bool NoTracks     () const;

	bool const IsValid( TrackNr const trackNr ) const;
	bool const IsValid( SignalId const & ) const;

	void InsertTrack( TrackNr const );
	void DeleteTrack( TrackNr const );

	void             AddSignal   ( TrackNr const, MicroMeterCircle const & );
	void             DeleteSignal( SignalId const & );
	SignalId const   MoveSignal  ( SignalId const &, TrackNr const );
	Signal   const & GetSignal   ( SignalId const & ) const;
	Signal         & GetSignal   ( SignalId const & );

	void Apply2AllTracks        ( TrackNrFunc const & ) const;
	void Apply2AllSignalsInTrack( TrackNr const, SignalNrFunc const & ) const;
	void Apply2AllSignals       ( SignalIdFunc const & ) const;
	void Apply2AllSignals       ( SignalFunc   const & ) const;

	Signal * const FindSignal( SignalCrit      const & ) const;
	Signal * const FindSensor( MicroMeterPoint const & ) const;

private:
	Track            & getTrack( TrackNr const );
	Track    const   & getTrack( TrackNr const ) const;
	SignalNr const     addSignal( TrackNr const, unique_ptr<Signal> );
	unique_ptr<Signal> removeSignal( SignalId const & );

	vector<unique_ptr<Track>> m_tracks {};
};
