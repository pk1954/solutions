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

using TrackNr        = NamedType< int, struct TrackNrParam >;
using TrackIter      = vector<Track>::iterator;
using TrackConstIter = vector<Track>::const_iterator;
using TrackFunc      = function<void(TrackConstIter const )>;

class MonitorData
{
public:
	void CheckTracks( ) const;  // for debugging

	void Initialize( Observable * const );
	void Reset( );

	int  GetNrOfTracks( ) const;
	bool NoTracks     ( ) const;

	bool const IsValid( TrackNr const trackNr ) const;

	SignalNr const GetSignalNr( TrackConstIter const, SignalIter const ) const;
	SignalNr const GetSignalNr( TrackNr const, SignalIter const ) const;

	TrackNr  const GetTrackNr( TrackConstIter const ) const;

	TrackIter const InsertTrack( TrackNr const = TrackNr(0) );
	void            DeleteTrack( TrackNr const = TrackNr(0) );

	Signal const * const GetSignal   ( TrackNr const, SignalNr const ) const;
	Signal       * const RemoveSignal( TrackNr const, SignalNr const );
	SignalNr       const AddSignal   ( TrackNr const, Signal * const );
	SignalNr       const MoveSignal  ( TrackNr const, SignalNr const, TrackNr const );

	void Apply2AllTracks ( TrackFunc const & ) const;
	void Apply2AllSignals( TrackNr const, SignalFunc const & ) const;

private:

	TrackIter      getTrack ( TrackNr const );
	TrackConstIter getTrackC( TrackNr const ) const;

	vector<Track> m_tracks { };
	Observable  * m_pStaticModelObservable { nullptr };
};
