// MonitorData.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Track.h"
#include "MonitorData.h"

using std::distance;
using std::move;

SignalId const SignalId::NULL_VAL { TrackNr::NULL_VAL(), SignalNr::NULL_VAL() };

void MonitorData::Initialize( Observable* const pStaticModelObservable )
{
	m_pStaticModelObservable = pStaticModelObservable;
}

void MonitorData::Reset( )
{
//	for ( Track & track: m_tracks ) { track.Clear(); };
	m_tracks.clear();
	m_pStaticModelObservable->NotifyAll( true );
}

int MonitorData::GetNrOfTracks( ) const
{
	return Cast2Int(m_tracks.size());
}

bool MonitorData::NoTracks( ) const
{
	return m_tracks.size() == 0;
}

void MonitorData::InsertTrack( TrackNr const trackNr )
{
	m_tracks.insert( m_tracks.begin() + trackNr.GetValue(), Track() );
	m_pStaticModelObservable->NotifyAll( true );
}

void MonitorData::DeleteSignal( SignalId const & id )
{
	if ( IsValid( id.GetTrackNr() ) )
	{
		getTrack( id.GetTrackNr() ).DeleteSignal( id.GetSignalNr() );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::AddSignal( unique_ptr<Signal> pSignal, TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
	{
		SignalNr signalNr { getTrack( trackNr ).AddSignal( move(pSignal) ) };
		m_pStaticModelObservable->NotifyAll( true );
	}
}

SignalId const MonitorData::MoveSignal(	SignalId const & id, TrackNr const trackNrNew )
{
	SignalId idNew { id };
	if ( IsValid( id ) && IsValid(trackNrNew) )
	{
		unique_ptr<Signal> pSignal     { getTrack( id.GetTrackNr() ).DeleteSignal( id.GetSignalNr() ) };
		SignalNr           signalNrNew { getTrack( trackNrNew ).AddSignal( move(pSignal) ) };
		idNew = SignalId( trackNrNew, signalNrNew );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return idNew;
}

void MonitorData::DeleteTrack( TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
	{
		m_tracks.erase( getTrackC( trackNr ) );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::Apply2AllTracks( TrackFunc const & func ) const
{
	for ( int i = 0; i < m_tracks.size(); ++i )
		func( TrackNr( i ) ); 
}                        

void MonitorData::Apply2AllSignals( TrackNr const trackNr, SignalFunc const & func ) const
{
	if ( IsValid( trackNr ) )
		getTrackC(trackNr)->Apply2AllSignals( func );
}                        

void MonitorData::Apply2AllSignals( function<void(SignalId const &)> const & func ) const
{
	Apply2AllTracks
	( 
		[&]( TrackNr const trackNr )
		{
			getTrackC(trackNr)->Apply2AllSignals
			(
				[&](SignalNr const & signalNr){	func( SignalId(trackNr, signalNr) ); }
			);
		}
	);
}                        

Signal const & MonitorData::GetSignal( SignalId const & id ) const
{
	assert( IsValid( id.GetTrackNr() ) );
	return getTrackC( id.GetTrackNr() )->GetSignal( id.GetSignalNr() );
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool const MonitorData::IsValid( SignalId const & id ) const
{
	return IsValid(id.GetTrackNr()) && getTrackC(id.GetTrackNr())->IsValid(id.GetSignalNr());
}

void MonitorData::CheckTracks( ) const
{
#ifdef _DEBUG
	for ( Track const & track: m_tracks ) 
		track.CheckSignals(); 
#endif
}

Track & MonitorData::getTrack( TrackNr const trackNr )
{
	assert( IsValid( trackNr ) );
	return m_tracks[trackNr.GetValue()]; 
}

vector<Track>::const_iterator const MonitorData::getTrackC( TrackNr const trackNr ) const 
{ 
	assert( IsValid( trackNr ) );
	return m_tracks.begin() + trackNr.GetValue(); 
}
