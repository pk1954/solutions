// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "Signal.h"
#include "Track.h"

using std::vector;

using TrackNr   = NamedType< int, struct TrackNrParam >;
using TrackFunc = function<void(TrackNr const)>;

struct SignalId
{
	SignalId( )
      : trackNr ( TrackNr ::NULL_VAL() ),
		signalNr( SignalNr::NULL_VAL() )
	{ }

	SignalId( TrackNr const tNr, SignalNr const sNr )
	  : trackNr( tNr),
		signalNr( sNr)
	{ }

	void Set2Null() 
	{ 
		trackNr.Set2Null(); 
		signalNr.Set2Null(); 
	}

	auto operator== (const SignalId & other) const
	{
		return (trackNr == other.trackNr) && (signalNr == other.signalNr);
	}

	auto operator!= (const SignalId & other) const
	{
		return ! (*this == other);
	}

	TrackNr  trackNr;
	SignalNr signalNr;
};

class MonitorData
{
public:

	void CheckTracks( ) const;  // for debugging

	void Initialize( Observable * const );
	void Reset( );

	int  GetNrOfTracks( ) const;
	bool NoTracks     ( ) const;

	bool const IsValid( TrackNr const trackNr ) const;
	bool const IsValid( SignalId const & ) const;

	void InsertTrack( TrackNr const = TrackNr(0) );
	void DeleteTrack( TrackNr const = TrackNr(0) );

	Signal const * const GetSignal   ( SignalId const & ) const;
	Signal       * const DeleteSignal( SignalId const & );
	SignalNr       const AddSignal   ( TrackNr const, Signal * const );
	SignalId       const MoveSignal  ( SignalId const &, TrackNr const );

	void Apply2AllTracks ( TrackFunc const & ) const;
	void Apply2AllSignals( TrackNr const, SignalFunc const & ) const;
	void Apply2AllSignals( function<void(SignalId const &)> const & ) const;

private:
	Track                       * const getTrack ( TrackNr const );
	vector<Track>::const_iterator const getTrackC( TrackNr const ) const;

	vector<Track> m_tracks { };
	Observable  * m_pStaticModelObservable { nullptr };
};
