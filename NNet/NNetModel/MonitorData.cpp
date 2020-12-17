// MonitorData.cpp 
//
// NNetWindows

#include "stdafx.h"
#include "Track.h"
#include "SignalFactory.h"
#include "MonitorData.h"

using std::distance;
using std::move;

void MonitorData::Initialize
( 
	Observable      * const pStaticModelObservable,
	SignalFactory   * const pSignalFactory
)
{
	m_pStaticModelObservable = pStaticModelObservable;
	m_pSignalFactory         = pSignalFactory;
}

void MonitorData::Reset( )
{
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

unique_ptr<Signal> MonitorData::removeSignal( SignalId const & id )
{ 
	return getTrack( id.GetTrackNr() ).RemoveSignal( id.GetSignalNr() ); 
};

SignalNr const MonitorData::addSignal( TrackNr const trackNr, unique_ptr<Signal> pSignal )
{
	return getTrack( trackNr ).AddSignal( move(pSignal) );
}

void MonitorData::DeleteSignal( SignalId const & id )
{
	if ( IsValid( id.GetTrackNr() ) )
	{
		removeSignal( id );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::AddSignal( TrackNr const trackNr, MicroMeterCircle const & umCircle )
{
	if ( IsValid( trackNr ) )
	{
		auto     upSignal { m_pSignalFactory->MakeSignal( umCircle ) };
		SignalNr signalNr { addSignal( trackNr, move(upSignal) ) };
		m_pStaticModelObservable->NotifyAll( true );
	}
}

SignalId const MonitorData::MoveSignal(	SignalId const & id, TrackNr const trackNrNew )
{
	SignalId idNew { id };
	if ( IsValid(id) && IsValid(trackNrNew) ) //-V1051
	{
		unique_ptr<Signal> pSignal     { removeSignal( id ) };
		SignalNr           signalNrNew { addSignal( trackNrNew, move(pSignal) ) };
		idNew = SignalId( trackNrNew, signalNrNew );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return idNew;
}

void MonitorData::DeleteTrack( TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
	{
		m_tracks.erase( m_tracks.begin() + trackNr.GetValue() );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::Animation( SignalId const & id, bool const bOn )
{
	if ( id.IsNotNull() )
		GetSignal( id ).Animate( bOn );
}

void MonitorData::Apply2AllTracks( TrackNrFunc const & func ) const
{
	for ( int i = 0; i < m_tracks.size(); ++i )
		func( TrackNr( i ) ); 
}                        

void MonitorData::Apply2AllSignalsInTrack( TrackNr const trackNr, SignalNrFunc const & func ) const
{
	if ( IsValid( trackNr ) )
		getTrack(trackNr).Apply2AllSignals( func );
}                        

void MonitorData::Apply2AllSignals( SignalIdFunc const & func ) const
{
	Apply2AllTracks
	( 
		[&]( TrackNr const trackNr )
		{
			getTrack(trackNr).Apply2AllSignals
			(
				[&](SignalNr const & signalNr){	func( SignalId(trackNr, signalNr) ); }
			);
		}
	);
}                        

void MonitorData::Apply2AllSignals( SignalFunc const & func ) const
{
	Apply2AllTracks
	( 
		[&]( TrackNr const trackNr )
		{
			getTrack(trackNr).Apply2AllSignals
			(
				[&](SignalNr const & signalNr)
				{	
					SignalId const   id     { SignalId(trackNr, signalNr) }; 
					Signal   const & signal { GetSignal( id )  }; 
					func( signal ); 
				}
			);
		}
	);
}                        

Signal * const  MonitorData::FindSignal( SignalCrit const & crit )
{
	for ( Track & track: m_tracks ) 
		if ( Signal * const pSignal { track.FindSignal( crit ) } )
			return pSignal;
	return nullptr;
}                        

Signal const & MonitorData::GetSignal( SignalId const & id ) const
{
	assert( IsValid( id.GetTrackNr() ) );
	return getTrack( id.GetTrackNr() ).GetSignal( id.GetSignalNr() );
}

Signal & MonitorData::GetSignal( SignalId const & id ) // calling const version 
{
	return const_cast<Signal&>(static_cast<const MonitorData&>(*this).GetSignal( id ));
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool const MonitorData::IsValid( SignalId const & id ) const
{
	return IsValid(id.GetTrackNr()) && getTrack(id.GetTrackNr()).IsValid(id.GetSignalNr());
}

void MonitorData::CheckTracks( ) const
{
#ifdef _DEBUG
	for ( Track const & track: m_tracks ) 
		track.CheckSignals(); 
#endif
}

Track const & MonitorData::getTrack( TrackNr const trackNr ) const
{
	assert( IsValid( trackNr ) );
	return m_tracks[trackNr.GetValue()]; 
}

Track & MonitorData::getTrack( TrackNr const trackNr ) // calling const version 
{
	return const_cast<Track&>(static_cast<const MonitorData&>(*this).getTrack( trackNr ));
}

Signal * const MonitorData::FindSensor( MicroMeterPoint const & umPos ) 
{
	return FindSignal
		   (
			   [&](Signal const & signal)
			   { 
				   return signal.Includes( umPos ); 
			   }
		   );
}
