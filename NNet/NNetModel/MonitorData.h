// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "Signal.h"

using std::vector;

class Signal;

class Track
{
public:
	void Clear( )
	{
		Apply2AllSignals( [&]( Signal & signal ) { delete & signal; } );
	}

	void AddSignal( Signal * const pSignal )
	{
		m_signals.push_back( pSignal );
	}

	void RemoveSignal( Signal * const pSignal )
	{
		auto itSignal { find( m_signals.begin(), m_signals.end(), pSignal ) };
		assert( itSignal != m_signals.end() );
		m_signals.erase( itSignal );
	}

	void Apply2AllSignalsC( function<void(Signal const &)> const & func ) const
	{
		for (auto pSignal : m_signals)
			if ( pSignal  )
				func( * pSignal ); 
	}                        

	void Apply2AllSignals( function<void(Signal &)> const & func )
	{
		for (auto pSignal : m_signals)
			if ( pSignal  )
				func( * pSignal ); 
	}                        

private:
	vector <Signal *> m_signals;
};

using TrackNr = NamedType< int, struct TrackNrParam >;

class MonitorData
{
public:

	void Initialize( Observable* const pStaticModelObservable )
	{
		m_pStaticModelObservable = pStaticModelObservable;
	}

	void Reset( )
	{
		Apply2AllTracks( [&](Track & track) { track.Clear(); } );
		m_Tracks.clear();
		m_pStaticModelObservable->NotifyAll( true );
	}

	int GetNrOfTracks( ) const
	{
		return CastToInt(m_Tracks.size());
	}

	bool NoTracks( ) const
	{
		return m_Tracks.size() == 0;
	}

	Track & GetTrack( TrackNr const trackNr )
	{
		assert( IsValid( trackNr ) );
		return m_Tracks[trackNr.GetValue()];
	}

	bool IsValid( TrackNr const trackNr ) const
	{
		return (trackNr.GetValue() >= 0) && (trackNr.GetValue() < m_Tracks.size());
	}

	vector<Track>::iterator InsertTrack( TrackNr const trackNr )
	{
		auto res { m_Tracks.insert( m_Tracks.begin() + trackNr.GetValue(), Track() ) };
		m_pStaticModelObservable->NotifyAll( true );
		return res;
	}

	vector<Track>::iterator AppendTrack( )
	{
		return InsertTrack(TrackNr(CastToInt(m_Tracks.size())) );
	}

	void RemoveSignalFromTrack( Signal * pSignal, TrackNr const trackNr )
	{
		if ( trackNr.IsNotNull() )
		{
			GetTrack( trackNr ).RemoveSignal( pSignal );
			m_pStaticModelObservable->NotifyAll( true );
		}
	}

	void DeleteSignal( Signal * pSignal, TrackNr const trackNr )
	{
		if ( trackNr.IsNotNull() )
		{
			RemoveSignalFromTrack( pSignal, trackNr );
			delete pSignal;
		}
	}

	void DeleteTrack( TrackNr const trackNr )
	{
		if ( trackNr.IsNotNull() )
		{
			GetTrack( trackNr ).Clear();
			m_Tracks.erase( m_Tracks.begin() + trackNr.GetValue() );
			m_pStaticModelObservable->NotifyAll( true );
		}
	}

	void DeleteYoungestTrack( )
	{
		if ( ! m_Tracks.empty() )
		{
			m_Tracks.pop_back();
			m_pStaticModelObservable->NotifyAll( true );
		}
	}

	void Apply2AllTracksC( function<void(Track const &)> const & func ) const
	{
		for (auto track : m_Tracks)    
			func( track ); 
	}                        

private:

	void Apply2AllTracks( function<void(Track &)> const & func )
	{
		for (auto track : m_Tracks)    
			func( track ); 
	}                        

	vector<Track> m_Tracks { };
	Observable  * m_pStaticModelObservable { nullptr };
};
