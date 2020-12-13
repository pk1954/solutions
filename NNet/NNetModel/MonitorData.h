// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "SignalInterface.h"
#include "Track.h"
#include "SignalId.h"

using std::vector;

using TrackFunc = function<void(TrackNr const)>;

class SignalFactory;

class MonitorData
{
public:

	void CheckTracks( ) const;  // for debugging

	void Initialize( Observable * const, SignalFactory * const );
	void Reset( );

	int  GetNrOfTracks( ) const;
	bool NoTracks     ( ) const;

	bool const IsValid( TrackNr const trackNr ) const;
	bool const IsValid( SignalId const & ) const;

	void InsertTrack( TrackNr const );
	void DeleteTrack( TrackNr const );

	void                    AddSignal   ( TrackNr const, ShapeId const );
	void                    AddSignal   ( TrackNr const, MicroMeterCircle const & );
	void                    DeleteSignal( SignalId const & );
	SignalId        const   MoveSignal  ( SignalId const &, TrackNr const );
	SignalInterface const & GetSignal   ( SignalId const & ) const;
	void                    Animation   ( SignalId const &, bool const );

	void Apply2AllTracks        ( TrackFunc const & ) const;
	void Apply2AllSignalsInTrack( TrackNr const, SignalFunc const & ) const;
	void Apply2AllSignals       ( function<void(SignalId const &)> const & ) const;

private:
	Track       & getTrack ( TrackNr const );
	Track const & getTrackC( TrackNr const ) const;
	unique_ptr<SignalInterface> removeSignal( SignalId const & );
	SignalNr const addSignal( TrackNr const, unique_ptr<SignalInterface> );

	vector<Track>   m_tracks                 { };
	Observable    * m_pStaticModelObservable { nullptr };
	SignalFactory * m_pSignalFactory         { nullptr };
};
